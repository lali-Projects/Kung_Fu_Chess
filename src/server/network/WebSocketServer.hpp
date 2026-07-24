#pragma once


#include "INetworkServer.hpp"

#include <memory>
#include <unordered_map>


class WebSocketServer : public INetworkServer
{

public:


    explicit WebSocketServer(
        unsigned short port);



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



private:


    /*
        Callback to upper server layer.

        Network layer does not know:
        - GameSession
        - GameEngine
        - Commands
    */
    MessageCallback m_callback;



    /*
        WebSocket connections.

        Key:
            connection id

        Value:
            websocket connection object

        Real type depends on library.
    */
    std::unordered_map<int, void*> m_connections;



    unsigned short m_port;



    bool m_running{false};

};