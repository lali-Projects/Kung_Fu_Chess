#include "WebSocketServer.hpp"


#include <iostream>
#include <utility>



//================================================
// Constructor
//================================================

WebSocketServer::WebSocketServer(
    uint16_t port)
:
m_port(port)
{

    m_server =
        std::make_unique<ix::WebSocketServer>(
            port);

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



    if(!m_configured)
    {
        configureServer();
        m_configured = true;
    }



    auto result =
        m_server->listen();



    if(!result.first)
    {

        std::cout
            << "WebSocket listen failed: "
            << result.second
            << std::endl;

        return;
    }



    m_server->start();



    m_running = true;



    std::cout
        << "WebSocket server started on port "
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



    {
        std::lock_guard<std::mutex> lock(
            m_connectionsMutex);



        for(auto& [id, socket] : m_connections)
        {

            if(socket)
            {
                socket->close();
            }

        }



        m_connections.clear();

    }



    m_server->stop();



    m_messageCallback = nullptr;
    m_connectionCallback = nullptr;
    m_disconnectCallback = nullptr;



    m_running = false;



    std::cout
        << "WebSocket server stopped"
        << std::endl;

}





//================================================
// Running
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

    std::shared_ptr<ix::WebSocket> socket;



    {

        std::lock_guard<std::mutex> lock(
            m_connectionsMutex);



        auto iterator =
            m_connections.find(
                connectionId);



        if(iterator ==
           m_connections.end())
        {
            return;
        }



        socket =
            iterator->second;

    }



    if(socket)
    {
        socket->send(
            message.getPayload());
    }

}





//================================================
// Disconnect
//================================================

void WebSocketServer::disconnect(
    int connectionId)
{

    std::shared_ptr<ix::WebSocket> socket;



    {

        std::lock_guard<std::mutex> lock(
            m_connectionsMutex);



        auto iterator =
            m_connections.find(
                connectionId);



        if(iterator ==
           m_connections.end())
        {
            return;
        }



        socket =
            iterator->second;



        m_connections.erase(
            iterator);

    }



    if(socket)
    {
        socket->close();
    }



    if(m_disconnectCallback)
    {
        m_disconnectCallback(
            connectionId);
    }



    std::cout
        << "Client disconnected: "
        << connectionId
        << std::endl;

}





//================================================
// Message Callback
//================================================

void WebSocketServer::setMessageCallback(
    MessageCallback callback)
{

    m_messageCallback =
        std::move(callback);

}





//================================================
// Connection Callback
//================================================

void WebSocketServer::setConnectionCallback(
    ConnectionCallback callback)
{

    m_connectionCallback =
        std::move(callback);

}





//================================================
// Configure Server
//================================================

void WebSocketServer::configureServer()
{

    m_server->setOnConnectionCallback(
        [this]
        (
            std::weak_ptr<ix::WebSocket> webSocketWeak,
            std::shared_ptr<ix::ConnectionState> connectionState
        )
        {

            handleClientConnection(
                webSocketWeak,
                connectionState);

        });

}





//================================================
// Handle Client Connection
//================================================

void WebSocketServer::handleClientConnection(
    std::weak_ptr<ix::WebSocket> webSocketWeak,
    std::shared_ptr<ix::ConnectionState>)
{

    auto webSocket =
        webSocketWeak.lock();



    if(!webSocket)
    {
        return;
    }



    int connectionId =
        m_nextConnectionId++;





    {
        std::lock_guard<std::mutex> lock(
            m_connectionsMutex);



        m_connections[connectionId] =
            webSocket;
    }





    /*
        IMPORTANT:

        Configure message callback
        BEFORE notifying upper layer.

        This guarantees that the socket
        is ready before Server creates
        ClientConnection.
    */


    webSocket->setOnMessageCallback(
        [this, connectionId]
        (
            const ix::WebSocketMessagePtr& msg
        )
        {
 std::cout
        << "[WEBSOCKET CALLBACK ENTER]"
        << std::endl;
            if(!msg)
            {
                return;
            }



            switch(msg->type)
            {


                case ix::WebSocketMessageType::Message:
                {

                    std::cout
                        << "[WEBSOCKET] Message: "
                        << msg->str
                        << std::endl;



                    handleMessage(
                        connectionId,
                        msg->str);


                    break;
                }



                case ix::WebSocketMessageType::Close:
                case ix::WebSocketMessageType::Error:
                {

                    removeConnection(
                        connectionId);


                    break;
                }



                default:
                    break;

            }

        });





    /*
        Notify server only after
        socket is ready.
    */

    if(m_connectionCallback)
    {

        m_connectionCallback(
            connectionId);

    }





    std::cout
        << "Client connected: "
        << connectionId
        << std::endl;

}





//================================================
// Handle Message
//================================================

void WebSocketServer::handleMessage(
    int connectionId,
    const std::string& payload)
{

    std::cout
        << "[HANDLE MESSAGE BEGIN] "
        << payload
        << std::endl;


    if(!m_messageCallback)
    {
        std::cout
            << "[NO MESSAGE CALLBACK]"
            << std::endl;

        return;
    }


    NetworkMessage message(
        MessageType::COMMAND,
        payload);


    m_messageCallback(
        connectionId,
        message);


    std::cout
        << "[HANDLE MESSAGE END] "
        << payload
        << std::endl;
}





//================================================
// Remove Connection
//================================================

void WebSocketServer::removeConnection(
    int connectionId)
{

    bool removed = false;



    {

        std::lock_guard<std::mutex> lock(
            m_connectionsMutex);



        auto iterator =
            m_connections.find(
                connectionId);



        if(iterator != m_connections.end())
        {

            m_connections.erase(
                iterator);


            removed = true;

        }

    }



    if(!removed)
    {
        return;
    }



    std::cout
        << "Client disconnected: "
        << connectionId
        << std::endl;



    if(m_disconnectCallback)
    {

        m_disconnectCallback(
            connectionId);

    }

}





//================================================
// Disconnect Callback
//================================================

void WebSocketServer::setDisconnectCallback(
    DisconnectCallback callback)
{

    m_disconnectCallback =
        std::move(callback);

}