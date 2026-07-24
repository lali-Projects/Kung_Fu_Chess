#pragma once


#include <functional>

#include "NetworkMessage.hpp"



/**
 * @brief Abstract network server interface.
 *
 * This layer hides the transport technology.
 *
 * Possible implementations:
 *
 *      LocalNetworkServer
 *      WebSocketServer
 *      TcpServer
 *
 *
 * Responsibilities:
 *
 *  - Start network service.
 *  - Stop network service.
 *  - Send messages to clients.
 *  - Disconnect clients.
 *  - Notify upper layers about incoming messages.
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
     * @brief Sets callback for incoming messages.
     *
     * Network layer calls this when:
     *
     * client
     *   |
     *   v
     * network
     *   |
     *   v
     * server
     */
    virtual void setMessageCallback(
        MessageCallback callback) = 0;

};