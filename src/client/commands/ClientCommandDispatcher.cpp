#include "ClientCommandDispatcher.hpp"

#include <future>
#include <sstream>
#include <utility>
#include <vector>

#include "AuthoritativeGameState.hpp"
#include "ClientSession.hpp"
#include "CommandResponseCodec.hpp"
#include "NetworkMessage.hpp"
#include "SnapshotDeserializer.hpp"


namespace client
{
    ClientCommandDispatcher::ClientCommandDispatcher(
        ITransport& transport,
        ClientSession& session,
        AuthoritativeGameState& gameState)
        : m_transport(transport),
          m_session(session),
          m_gameState(gameState)
    {
        m_transport.setMessageHandler(
            [this](const std::string& rawMessage)
            {
                handleRawMessage(rawMessage);
            });

        m_transport.setStateHandler(
            [this](TransportState state, const std::string& reason)
            {
                handleTransportState(state, reason);
            });
    }


    ClientCommandDispatcher::~ClientCommandDispatcher()
    {
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            m_stopping = true;
        }

        // Clear callbacks before stopping the transport, then wait for the
        // implementation to finish its callback thread.
        m_transport.setMessageHandler(nullptr);
        m_transport.setStateHandler(nullptr);
        m_transport.disconnect();
        failAll("dispatcher_stopped");
    }


    void ClientCommandDispatcher::sendCommand(
        std::string command,
        ResponseHandler handler)
    {
        if(command.empty())
        {
            if(handler)
            {
                handler(failure("", "empty_command"));
            }
            return;
        }

        const bool connected = m_transport.isConnected();
        bool dispatch = false;
        bool reject = false;
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            reject = m_stopping || !connected;
            if(!reject)
            {
                m_pending.push_back(
                    {std::move(command), std::move(handler)});
                dispatch = m_pending.size() == 1;
            }
        }

        if(reject)
        {
            if(handler)
            {
                handler(failure(command, "not_connected"));
            }
            return;
        }

        if(dispatch)
        {
            dispatchFront();
        }
    }


    CommandResponse ClientCommandDispatcher::sendCommandAndWait(
        std::string command,
        std::chrono::milliseconds timeout)
    {
        const std::string originalCommand = command;
        auto completion =
            std::make_shared<std::promise<CommandResponse>>();
        std::future<CommandResponse> result =
            completion->get_future();

        sendCommand(
            std::move(command),
            [completion](const CommandResponse& response)
            {
                try
                {
                    completion->set_value(response);
                }
                catch(const std::future_error&)
                {
                    // A timeout/disconnect may already have completed it.
                }
            });

        if(result.wait_for(timeout) != std::future_status::ready)
        {
            failAll("command_timeout");
            m_transport.disconnect();
        }

        if(result.wait_for(std::chrono::milliseconds(0)) ==
           std::future_status::ready)
        {
            return result.get();
        }

        return failure(originalCommand, "command_timeout");
    }


    void ClientCommandDispatcher::setErrorHandler(
        ErrorHandler handler)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_errorHandler = std::move(handler);
    }


    void ClientCommandDispatcher::setConnectionHandler(
        ConnectionHandler handler)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_connectionHandler = std::move(handler);
    }


    void ClientCommandDispatcher::dispatchFront()
    {
        std::string command;
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            if(m_stopping || m_pending.empty())
            {
                return;
            }
            command = m_pending.front().command;
        }

        const NetworkMessage message(
            MessageType::COMMAND,
            command);
        if(!m_transport.send(message.serialize()))
        {
            failAll("send_failed");
            m_transport.disconnect();
        }
    }


    void ClientCommandDispatcher::completeFront(
        CommandResponse response)
    {
        const bool hasExplicitClickSelection =
            response.command == "CLICK";

        PendingCommand completed;
        bool hasCompleted = false;
        bool hasNext = false;
        bool commandMismatch = false;
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            if(!m_pending.empty())
            {
                const std::string expected =
                    commandName(m_pending.front().command);
                commandMismatch =
                    !response.command.empty() &&
                    response.command != expected;

                if(!commandMismatch)
                {
                    completed = std::move(m_pending.front());
                    m_pending.pop_front();
                    hasCompleted = true;
                    hasNext = !m_pending.empty();
                }
            }
        }

        if(commandMismatch)
        {
            reportError("command_response_mismatch");
            failAll("command_response_mismatch");
            m_transport.disconnect();
            return;
        }

        if(!hasCompleted)
        {
            reportError("unsolicited_command_result");
            return;
        }

        if(response.command.empty())
        {
            response.command = commandName(completed.command);
        }

        // CLICK replies carry the authoritative per-player selection. Other
        // command replies intentionally leave that presentation state alone.
        // In particular, an unparsed command has no explicit command name and
        // cannot safely imply that the server cleared its stored selection.
        if(hasExplicitClickSelection)
        {
            m_gameState.setSelectedPosition(
                response.selectedPosition);
        }

        if(completed.handler)
        {
            completed.handler(response);
        }

        if(hasNext)
        {
            dispatchFront();
        }
    }


    void ClientCommandDispatcher::failAll(
        const std::string& reason)
    {
        std::deque<PendingCommand> pending;
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            pending.swap(m_pending);
        }

        for(auto& item : pending)
        {
            if(item.handler)
            {
                item.handler(
                    failure(item.command, reason));
            }
        }
    }


    void ClientCommandDispatcher::handleRawMessage(
        const std::string& rawMessage)
    {
        const auto message =
            NetworkMessage::deserialize(rawMessage);
        if(!message)
        {
            reportError("malformed_network_message");
            return;
        }
        handleMessage(*message);
    }


    void ClientCommandDispatcher::handleMessage(
        const NetworkMessage& message)
    {
        switch(message.getType())
        {
            case MessageType::COMMAND_RESULT:
            {
                const auto response =
                    CommandResponseCodec::deserialize(
                        message.getPayload());
                if(!response)
                {
                    completeFront(
                        failure("", "malformed_command_response"));
                    return;
                }
                completeFront(*response);
                return;
            }

            case MessageType::GAME_STATE:
            {
                auto snapshot =
                    SnapshotDeserializer::deserialize(message);
                if(!snapshot)
                {
                    reportError("malformed_game_snapshot");
                    return;
                }

                // Serialize the final connection check and state update with
                // disconnect clearing. Whichever transition wins is then
                // completed atomically from the dispatcher's perspective.
                std::lock_guard<std::mutex> stateLock(
                    m_stateTransitionMutex);
                if(m_session.isConnected())
                {
                    m_gameState.update(std::move(*snapshot));
                }
                return;
            }

            case MessageType::SYSTEM_ERROR:
                completeFront(
                    failure("", message.getPayload()));
                return;

            case MessageType::DISCONNECT:
            {
                const std::string reason =
                    message.getPayload().empty()
                        ? "server_disconnect"
                        : message.getPayload();
                {
                    std::lock_guard<std::mutex> lock(m_mutex);
                    m_requestedDisconnectReason = reason;
                }
                m_transport.disconnect();
                return;
            }

            case MessageType::HEARTBEAT:
                return;

            default:
                reportError("unexpected_message_type");
                return;
        }
    }


    void ClientCommandDispatcher::handleTransportState(
        TransportState state,
        const std::string& reason)
    {
        std::string effectiveReason = reason;
        if(state == TransportState::DISCONNECTED)
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            if(!m_requestedDisconnectReason.empty())
            {
                effectiveReason =
                    std::move(m_requestedDisconnectReason);
                m_requestedDisconnectReason.clear();
            }
        }

        if(state == TransportState::CONNECTED)
        {
            std::lock_guard<std::mutex> stateLock(
                m_stateTransitionMutex);
            m_session.setConnected(true);
        }
        else if(state == TransportState::CONNECTING)
        {
            std::lock_guard<std::mutex> stateLock(
                m_stateTransitionMutex);
            m_session.setConnected(false);
        }
        else
        {
            {
                std::lock_guard<std::mutex> stateLock(
                    m_stateTransitionMutex);
                m_session.setConnected(false);
                m_gameState.clear();
            }
            failAll(
                effectiveReason.empty()
                    ? "disconnected"
                    : effectiveReason);
        }

        ConnectionHandler handler;
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            handler = m_connectionHandler;
        }
        if(handler)
        {
            handler(state, effectiveReason);
        }
    }


    void ClientCommandDispatcher::reportError(
        const std::string& reason)
    {
        ErrorHandler handler;
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            handler = m_errorHandler;
        }
        if(handler)
        {
            handler(reason);
        }
    }


    CommandResponse ClientCommandDispatcher::failure(
        const std::string& command,
        const std::string& reason)
    {
        CommandResponse response;
        response.success = false;
        response.reason = reason;
        response.command = commandName(command);
        return response;
    }


    std::string ClientCommandDispatcher::commandName(
        const std::string& command)
    {
        std::istringstream stream(command);
        std::string name;
        stream >> name;
        return name;
    }
}
