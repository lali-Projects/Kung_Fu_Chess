#include "ClientSocket.hpp"

#include <chrono>
#include <iostream>
#include <utility>


ClientSocket::ClientSocket()
{
    // Session state and command replay are owned by the client layer. An
    // implicit reconnect could otherwise revive a socket with stale identity.
    m_socket.disableAutomaticReconnection();
}


ClientSocket::~ClientSocket()
{
    disconnect();
}


bool ClientSocket::connect(const std::string& url)
{
    if(url.empty())
    {
        updateState(
            client::TransportState::FAILED,
            "empty_url");
        return false;
    }

    if(isConnected())
    {
        return true;
    }

    // Release resources left by a failed or remotely closed attempt.
    disconnect();

    m_socket.setUrl(url);
    m_socket.setOnMessageCallback(
        [this](const ix::WebSocketMessagePtr& message)
        {
            {
                std::lock_guard<std::mutex> lock(m_mutex);
                m_callbackThreadId = std::this_thread::get_id();
            }

            if(!message)
            {
                return;
            }

            switch(message->type)
            {
                case ix::WebSocketMessageType::Open:
                    if(updateState(
                           client::TransportState::CONNECTED,
                           "connected"))
                    {
                        std::cout
                            << "[CLIENT] WebSocket connected\n";
                    }
                    break;

                case ix::WebSocketMessageType::Close:
                {
                    client::TransportState previous;
                    {
                        std::lock_guard<std::mutex> lock(m_mutex);
                        previous = m_state;
                    }

                    const client::TransportState nextState =
                        previous == client::TransportState::CONNECTING
                            ? client::TransportState::FAILED
                            : previous == client::TransportState::FAILED
                                ? client::TransportState::FAILED
                                : client::TransportState::DISCONNECTED;
                    const std::string reason =
                        previous == client::TransportState::CONNECTING
                            ? "connection_closed"
                            : previous == client::TransportState::FAILED
                                ? "connection_failed"
                                : "remote_disconnect";

                    if(updateState(nextState, reason))
                    {
                        std::cout
                            << "[CLIENT] WebSocket closed\n";
                    }
                    break;
                }

                case ix::WebSocketMessageType::Error:
                {
                    const std::string reason =
                        message->errorInfo.reason.empty()
                            ? "websocket_error"
                            : message->errorInfo.reason;
                    if(updateState(
                           client::TransportState::FAILED,
                           reason))
                    {
                        std::cout
                            << "[CLIENT] WebSocket error: "
                            << reason
                            << "\n";
                    }
                    break;
                }

                case ix::WebSocketMessageType::Message:
                {
                    MessageHandler handler;
                    {
                        std::lock_guard<std::mutex> lock(m_mutex);
                        handler = m_messageHandler;
                    }
                    if(handler)
                    {
                        handler(message->str);
                    }
                    break;
                }

                default:
                    break;
            }
        });

    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_started = true;
        m_callbackThreadId = std::thread::id{};
    }
    updateState(
        client::TransportState::CONNECTING,
        "connecting");

    try
    {
        m_socket.start();
    }
    catch(const std::exception& exception)
    {
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            m_started = false;
        }
        updateState(
            client::TransportState::FAILED,
            exception.what());
        return false;
    }

    std::unique_lock<std::mutex> lock(m_mutex);
    const bool completed =
        m_connectionCondition.wait_for(
            lock,
            std::chrono::seconds(5),
            [this]()
            {
                return m_state != client::TransportState::CONNECTING;
            });
    const bool connected =
        completed &&
        m_state == client::TransportState::CONNECTED;
    lock.unlock();

    if(!connected)
    {
        const bool timedOut = !completed;
        if(timedOut)
        {
            std::cout << "[CLIENT] Connection timeout\n";
            updateState(
                client::TransportState::FAILED,
                "connection_timeout");
        }

        {
            std::lock_guard<std::mutex> stateLock(m_mutex);
            m_started = false;
        }
        m_socket.stop();

        return false;
    }

    return true;
}


void ClientSocket::disconnect()
{
    bool shouldStop;
    bool calledFromCallback;
    client::TransportState previous;
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        shouldStop = m_started;
        calledFromCallback =
            shouldStop &&
            m_callbackThreadId == std::this_thread::get_id();
        if(!calledFromCallback)
        {
            m_started = false;
        }
        previous = m_state;
    }

    if(previous != client::TransportState::DISCONNECTED)
    {
        // Transition first. IXWebSocket::stop/close can synchronously emit a
        // Close event; updateState then suppresses that duplicate transition.
        updateState(
            client::TransportState::DISCONNECTED,
            "client_disconnect");
    }

    if(shouldStop)
    {
        if(calledFromCallback)
        {
            // IXWebSocket::stop joins its worker. From that worker, request
            // close only; a later owner-thread disconnect/destructor joins it.
            m_socket.close();
        }
        else
        {
            m_socket.stop();
        }
    }

}


bool ClientSocket::isConnected() const
{
    std::lock_guard<std::mutex> lock(m_mutex);
    return m_state == client::TransportState::CONNECTED;
}


client::TransportState ClientSocket::getState() const
{
    std::lock_guard<std::mutex> lock(m_mutex);
    return m_state;
}


bool ClientSocket::send(const std::string& message)
{
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        if(m_state != client::TransportState::CONNECTED)
        {
            return false;
        }
    }
    return m_socket.send(message).success;
}


void ClientSocket::setMessageHandler(MessageHandler handler)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    m_messageHandler = std::move(handler);
}


void ClientSocket::setStateHandler(StateHandler handler)
{
    client::TransportState state;
    StateHandler installed;
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_stateHandler = std::move(handler);
        installed = m_stateHandler;
        state = m_state;
    }

    if(installed)
    {
        installed(state, "current_state");
    }
}


bool ClientSocket::updateState(
    client::TransportState state,
    const std::string& reason)
{
    StateHandler handler;
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        if(m_state == state)
        {
            return false;
        }
        m_state = state;
        handler = m_stateHandler;
    }

    m_connectionCondition.notify_all();
    if(handler)
    {
        handler(state, reason);
    }
    return true;
}
