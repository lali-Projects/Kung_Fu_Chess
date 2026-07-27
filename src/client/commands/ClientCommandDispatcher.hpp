#pragma once

#include <deque>
#include <functional>
#include <mutex>
#include <string>

#include "IClientCommandGateway.hpp"
#include "ITransport.hpp"


class AuthoritativeGameState;
class ClientSession;
class NetworkMessage;


namespace client
{
    /**
     * Owns the single transport receive callback, preserves command ordering,
     * and demultiplexes command replies from asynchronous game snapshots.
     */
    class ClientCommandDispatcher : public IClientCommandGateway
    {
    public:
        using ErrorHandler =
            std::function<void(const std::string&)>;

        using ConnectionHandler =
            std::function<void(
                TransportState,
                const std::string&)>;

        ClientCommandDispatcher(
            ITransport& transport,
            ClientSession& session,
            AuthoritativeGameState& gameState);

        ~ClientCommandDispatcher() override;

        ClientCommandDispatcher(
            const ClientCommandDispatcher&) = delete;
        ClientCommandDispatcher& operator=(
            const ClientCommandDispatcher&) = delete;

        void sendCommand(
            std::string command,
            ResponseHandler handler = nullptr) override;

        CommandResponse sendCommandAndWait(
            std::string command,
            std::chrono::milliseconds timeout =
                std::chrono::seconds(5)) override;

        void setErrorHandler(ErrorHandler handler);
        void setConnectionHandler(ConnectionHandler handler);

    private:
        struct PendingCommand
        {
            std::string command;
            ResponseHandler handler;
        };

        void dispatchFront();
        void completeFront(CommandResponse response);
        void failAll(const std::string& reason);
        void handleRawMessage(const std::string& rawMessage);
        void handleMessage(const NetworkMessage& message);
        void handleTransportState(
            TransportState state,
            const std::string& reason);
        void reportError(const std::string& reason);

        static CommandResponse failure(
            const std::string& command,
            const std::string& reason);

        static std::string commandName(
            const std::string& command);

    private:
        ITransport& m_transport;
        ClientSession& m_session;
        AuthoritativeGameState& m_gameState;
        std::mutex m_stateTransitionMutex;
        std::mutex m_mutex;
        std::deque<PendingCommand> m_pending;
        ErrorHandler m_errorHandler;
        ConnectionHandler m_connectionHandler;
        std::string m_requestedDisconnectReason;
        bool m_stopping{false};
    };
}
