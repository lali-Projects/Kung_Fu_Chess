#pragma once


#include <functional>

#include "NetworkMessage.hpp"





/**
 * @brief Abstract network server interface.
 *
 * Hides transport implementation.
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
        std::function<NetworkMessage(
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


    virtual void start() = 0;



    virtual void stop() = 0;



    virtual bool isRunning() const = 0;





    virtual void send(
        int connectionId,
        const NetworkMessage& message) = 0;





    virtual void disconnect(
        int connectionId) = 0;





    /**
     * Client -> Server
     *
     * Returns:
     *
     * Server -> Client response
     */
    virtual void setMessageCallback(
        MessageCallback callback) = 0;





    virtual void setConnectionCallback(
        ConnectionCallback callback) = 0;





    virtual void setDisconnectCallback(
        DisconnectCallback callback) = 0;


};