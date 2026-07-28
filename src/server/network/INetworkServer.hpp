#pragma once

#include <functional>
#include "NetworkMessage.hpp"

/**
 * @brief Abstract interface for network server implementations.
 * 
 * Decouples network transport from core application/game logic.
 * 
 * Responsibilities:
 * - Manage network lifecycle (start, stop, query status).
 * - Transmit messages and manage client connections/disconnections.
 * - Provide callback registration for network events. 
 */
class INetworkServer
{
public:
    using MessageCallback    = std::function<NetworkMessage(int connectionId, const NetworkMessage& message)>;
    using ConnectionCallback = std::function<void(int connectionId)>;
    using DisconnectCallback = std::function<void(int connectionId)>;

    virtual ~INetworkServer() = default;

    INetworkServer() = default;

    INetworkServer(const INetworkServer&) = delete;
    INetworkServer& operator=(const INetworkServer&) = delete;

    virtual void start() = 0;
    virtual void stop() = 0;
    virtual bool isRunning() const = 0;

    virtual void send(int connectionId, const NetworkMessage& message) = 0;
    virtual void disconnect(int connectionId) = 0;

    virtual void setMessageCallback(MessageCallback callback) = 0;
    virtual void setConnectionCallback(ConnectionCallback callback) = 0;
    virtual void setDisconnectCallback(DisconnectCallback callback) = 0;
};