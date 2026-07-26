#pragma once


#include <atomic>
#include <map>
#include <optional>


#include "INetworkServer.hpp"



/**
 * @brief Local implementation of INetworkServer.
 *
 * Development transport layer.
 *
 * Simulates real network flow.
 *
 * Used for:
 *
 *  - Integration tests.
 *  - Server pipeline validation.
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


    //---------------------------------
    // INetworkServer
    //---------------------------------

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



    void setConnectionCallback(
        ConnectionCallback callback) override;



    void setDisconnectCallback(
        DisconnectCallback callback) override;



public:


    //---------------------------------
    // Test Helpers
    //---------------------------------


    int simulateNewConnection();



    void simulateIncomingMessage(
        int connectionId,
        const NetworkMessage& message);



    /**
     * @brief Returns last message sent to client.
     *
     * Testing only.
     */
    std::optional<NetworkMessage>
    getLastSentMessage(
        int connectionId) const;



    /**
     * @brief Clears stored messages.
     *
     * Testing only.
     */
    void clearSentMessages();



private:


    bool m_running{false};



    MessageCallback
        m_messageCallback;



    ConnectionCallback
        m_connectionCallback;



    DisconnectCallback
        m_disconnectCallback;



    std::atomic<int>
        m_nextConnectionId{1};



    /*
        Testing storage only.
    */
    std::map<
        int,
        NetworkMessage>
        m_sentMessages;

};