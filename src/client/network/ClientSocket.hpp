#pragma once

#include <condition_variable>
#include <mutex>
#include <string>
#include <thread>

#include <ixwebsocket/IXWebSocket.h>

#include "ITransport.hpp"


class ClientSocket : public client::ITransport
{
public:
    ClientSocket();
    ~ClientSocket() override;

    ClientSocket(const ClientSocket&) = delete;
    ClientSocket& operator=(const ClientSocket&) = delete;

    bool connect(const std::string& url) override;
    void disconnect() override;
    bool isConnected() const override;
    client::TransportState getState() const override;
    bool send(const std::string& message) override;
    void setMessageHandler(MessageHandler handler) override;
    void setStateHandler(StateHandler handler) override;

private:
    bool updateState(
        client::TransportState state,
        const std::string& reason);

private:
    ix::WebSocket m_socket;
    MessageHandler m_messageHandler;
    StateHandler m_stateHandler;
    mutable std::mutex m_mutex;
    std::condition_variable m_connectionCondition;
    client::TransportState m_state{
        client::TransportState::DISCONNECTED
    };
    std::thread::id m_callbackThreadId;
    bool m_started{false};
};
