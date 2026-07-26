#pragma once


#include <functional>

#include "NetworkMessage.hpp"



/**
 * @brief Abstract network server interface.
 *
 * Hides transport implementation.
 *
 * Implementations:
 *
 *      LocalNetworkServer
 *      WebSocketServer
 *
 *
 * Responsibilities:
 *
 *  - Start network service.
 *  - Stop network service.
 *  - Send messages.
 *  - Disconnect clients.
 *  - Notify about connections.
 *  - Notify about disconnections.
 *
 *
 * Does NOT know:
 *
 *  - Game rules.
 *  - Board.
 *  - GameEngine.
 *  - Sessions.
 *  - Players.
 */
class INetworkServer
{

public:


    using MessageCallback =
        std::function<void(
            int connectionId,
            const NetworkMessage& message)>;



    using ConnectionCallback =
        std::function<void(
            int connectionId)>;



    using DisconnectCallback =
        std::function<void(
            int connectionId)>;



public:


    virtual ~INetworkServer() = default;



    INetworkServer() = default;



    INetworkServer(
        const INetworkServer&) = delete;



    INetworkServer& operator=(
        const INetworkServer&) = delete;



public:


    /**
     * @brief Starts network service.
     */
    virtual void start() = 0;



    /**
     * @brief Stops network service.
     */
    virtual void stop() = 0;



    /**
     * @brief Checks server state.
     */
    virtual bool isRunning() const = 0;



    /**
     * @brief Sends message to specific client.
     */
    virtual void send(
        int connectionId,
        const NetworkMessage& message) = 0;



    /**
     * @brief Disconnects client.
     */
    virtual void disconnect(
        int connectionId) = 0;



    /**
     * @brief Called when client sends message.
     *
     * Flow:
     *
     * Client
     *    |
     *    v
     * Network layer
     *    |
     *    v
     * MessageCallback
     *    |
     *    v
     * Server
     */
    virtual void setMessageCallback(
        MessageCallback callback) = 0;



    /**
     * @brief Called when new client connects.
     *
     * Flow:
     *
     * Client
     *    |
     *    v
     * Network layer
     *    |
     *    v
     * ConnectionCallback
     *    |
     *    v
     * Server
     */
    virtual void setConnectionCallback(
        ConnectionCallback callback) = 0;



    /**
     * @brief Called when client disconnects.
     *
     * Flow:
     *
     * Client
     *    |
     *    v
     * Network layer
     *    |
     *    v
     * DisconnectCallback
     *    |
     *    v
     * Server
     */
    virtual void setDisconnectCallback(
        DisconnectCallback callback) = 0;

};