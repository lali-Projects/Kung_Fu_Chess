#pragma once


#include "INetworkServer.hpp"



/**
 * @brief Local implementation of INetworkServer.
 *
 * Development transport layer.
 *
 * Responsibilities:
 *
 *  - Simulate server lifecycle.
 *  - Send messages to clients.
 *  - Receive simulated messages.
 *  - Notify upper layers through callback.
 *
 *
 * Does NOT know:
 *
 *  - Game rules.
 *  - GameEngine.
 *  - Board.
 *  - Sessions.
 *  - Serialization.
 */
class LocalNetworkServer
    :
    public INetworkServer
{

public:


    LocalNetworkServer();



    ~LocalNetworkServer() override = default;



    LocalNetworkServer(
        const LocalNetworkServer&) = delete;



    LocalNetworkServer& operator=(
        const LocalNetworkServer&) = delete;



public:


    void start() override;



    void stop() override;



    bool isRunning() const override;



    void send(
        int connectionId,
        const NetworkMessage& message) override;



    void disconnect(
        int connectionId) override;



    void setMessageCallback(
        MessageCallback callback) override;



public:


    /**
     * @brief Development only.
     *
     * Simulates incoming network packet.
     */
    void simulateIncomingMessage(
        int connectionId,
        const NetworkMessage& message);



private:


    bool m_running{false};



    MessageCallback m_messageCallback;

};