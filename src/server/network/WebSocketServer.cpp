#include "WebSocketServer.hpp"


#include <iostream>



//================================================
// Constructor
//================================================

WebSocketServer::WebSocketServer(
    unsigned short port)
:
m_port(port)
{
}



//================================================
// Destructor
//================================================

WebSocketServer::~WebSocketServer()
{
    stop();
}



//================================================
// Start
//================================================

void WebSocketServer::start()
{

    if(m_running)
    {
        return;
    }


    m_running = true;


    std::cout
        << "WebSocketServer started on port "
        << m_port
        << std::endl;

}



//================================================
// Stop
//================================================

void WebSocketServer::stop()
{

    if(!m_running)
    {
        return;
    }


    m_running = false;


    m_connections.clear();


    std::cout
        << "WebSocketServer stopped"
        << std::endl;

}



//================================================
// Is Running
//================================================

bool WebSocketServer::isRunning() const
{
    return m_running;
}



//================================================
// Send
//================================================

void WebSocketServer::send(
    int connectionId,
    const NetworkMessage& message)
{

    if(!m_running)
    {
        return;
    }



    auto iterator =
        m_connections.find(
            connectionId);



    if(iterator == m_connections.end())
    {
        return;
    }



    /*
        Here will be:

        websocket.send(
            message.getPayload()
        );

        using ixwebsocket
    */


    std::cout
        << "[WebSocket Send] "
        << connectionId
        << " : "
        << message.getPayload()
        << std::endl;

}



//================================================
// Disconnect
//================================================

void WebSocketServer::disconnect(
    int connectionId)
{

    auto iterator =
        m_connections.find(
            connectionId);



    if(iterator ==
       m_connections.end())
    {
        return;
    }



    /*
        Later:

        websocket.close();
    */


    m_connections.erase(
        iterator);



    std::cout
        << "Disconnected client "
        << connectionId
        << std::endl;

}



//================================================
// Set Callback
//================================================

void WebSocketServer::setMessageCallback(
    MessageCallback callback)
{
    m_callback =
        std::move(callback);
}