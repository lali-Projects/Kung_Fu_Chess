#pragma once


#include <atomic>
#include <cstdint>
#include <memory>
#include <mutex>
#include <unordered_map>


#include <ixwebsocket/IXWebSocketServer.h>
#include <ixwebsocket/IXWebSocket.h>
#include <ixwebsocket/IXConnectionState.h>


#include "INetworkServer.hpp"



class WebSocketServer : public INetworkServer
{

public:


    explicit WebSocketServer(
        uint16_t port = 8080);



    ~WebSocketServer() override;



    WebSocketServer(
        const WebSocketServer&) = delete;



    WebSocketServer& operator=(
        const WebSocketServer&) = delete;




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




    void setConnectionCallback(
        ConnectionCallback callback) override;




    void setDisconnectCallback(
        DisconnectCallback callback) override;





private:


    void configureServer();



    void handleClientConnection(
        std::weak_ptr<ix::WebSocket> webSocket,
        std::shared_ptr<ix::ConnectionState> connectionState);



    void handleMessage(
        int connectionId,
        const std::string& payload);



    void removeConnection(
        int connectionId);





private:


    uint16_t m_port;



    std::unique_ptr<ix::WebSocketServer>
        m_server;




    std::atomic<bool>
        m_running{false};




    bool m_configured{false};




    MessageCallback
        m_messageCallback;




    ConnectionCallback
        m_connectionCallback;




    DisconnectCallback
        m_disconnectCallback;




    std::mutex
        m_connectionsMutex;


    std::mutex
        m_callbackMutex;




    std::unordered_map<
        int,
        std::shared_ptr<ix::WebSocket>>
        m_connections;




    std::atomic<int>
        m_nextConnectionId{1};

};
