#include "WebSocketServer.hpp"


#include <iostream>
#include <stdexcept>
#include <utility>
#include <vector>



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
        throw std::runtime_error(
            "WebSocket listen failed: " +
            result.second);
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



    std::vector<std::shared_ptr<ix::WebSocket>> sockets;


    {
        std::lock_guard<std::mutex> lock(
            m_connectionsMutex);



        for(auto& [id, socket] : m_connections)
        {

            if(socket)
            {
                sockets.push_back(socket);
            }

        }



        m_connections.clear();

    }


    for(const auto& socket : sockets)
    {
        socket->close();
    }



    m_server->stop();



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
            /*
                Wire framing:

                Always send the fully framed
                "TYPE|payload" form so the client
                can tell a COMMAND_RESULT reply
                apart from an async GAME_STATE
                broadcast. Both arrive on the same
                socket, interleaved.
            */

            message.serialize());
    }

}





//================================================
// Disconnect
//================================================

void WebSocketServer::disconnect(
    int connectionId)
{

    std::shared_ptr<ix::WebSocket> socket;
    DisconnectCallback disconnectCallback;



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


    {
        std::lock_guard<std::mutex> lock(
            m_callbackMutex);


        disconnectCallback =
            m_disconnectCallback;
    }



    if(disconnectCallback)
    {
        try
        {
            disconnectCallback(
                connectionId);
        }
        catch(const std::exception& exception)
        {
            std::cerr
                << "Disconnect callback failed: "
                << exception.what()
                << std::endl;
        }
        catch(...)
        {
            std::cerr
                << "Disconnect callback failed"
                << std::endl;
        }
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

    std::lock_guard<std::mutex> lock(
        m_callbackMutex);

    m_messageCallback =
        std::move(callback);

}





//================================================
// Connection Callback
//================================================

void WebSocketServer::setConnectionCallback(
    ConnectionCallback callback)
{

    std::lock_guard<std::mutex> lock(
        m_callbackMutex);

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
            if(!msg)
            {
                return;
            }



            switch(msg->type)
            {


                case ix::WebSocketMessageType::Message:
                {
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

    ConnectionCallback connectionCallback;


    {
        std::lock_guard<std::mutex> lock(
            m_callbackMutex);


        connectionCallback =
            m_connectionCallback;
    }


    if(connectionCallback)
    {
        try
        {
            connectionCallback(
                connectionId);
        }
        catch(const std::exception& exception)
        {
            std::cerr
                << "Connection callback failed: "
                << exception.what()
                << std::endl;


            send(
                connectionId,
                NetworkMessage(
                    MessageType::SYSTEM_ERROR,
                    "internal_server_error"));


            removeConnection(
                connectionId);


            return;
        }
        catch(...)
        {
            std::cerr
                << "Connection callback failed"
                << std::endl;


            send(
                connectionId,
                NetworkMessage(
                    MessageType::SYSTEM_ERROR,
                    "internal_server_error"));


            removeConnection(
                connectionId);


            return;
        }

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

    try
    {
        MessageCallback messageCallback;


        {
            std::lock_guard<std::mutex> lock(
                m_callbackMutex);


            messageCallback =
                m_messageCallback;
        }


        if(!messageCallback)
        {
            std::cout
                << "[NO MESSAGE CALLBACK]"
                << std::endl;

            return;
        }



        /*
            Wire framing:

            The client now always sends a
            fully framed "TYPE|payload" string
            (see AuthenticationClient / any future
            gameplay client). Reject anything that
            does not parse as a well-formed COMMAND
            instead of silently treating raw bytes
            as a command payload.
        */

        auto parsed =
            NetworkMessage::deserialize(payload);


        if(!parsed ||
           parsed->getType() != MessageType::COMMAND)
        {

            send(
                connectionId,
                NetworkMessage(
                    MessageType::SYSTEM_ERROR,
                    "malformed_message"));

            return;
        }


        NetworkMessage response =
            messageCallback(
                connectionId,
                *parsed);


        send(
            connectionId,
            response);


    }
    catch(const std::exception& exception)
    {
        std::cerr
            << "Message handling failed: "
            << exception.what()
            << std::endl;


        send(
            connectionId,
            NetworkMessage(
                MessageType::SYSTEM_ERROR,
                "internal_server_error"));
    }
    catch(...)
    {
        std::cerr
            << "Message handling failed"
            << std::endl;


        send(
            connectionId,
            NetworkMessage(
                MessageType::SYSTEM_ERROR,
                "internal_server_error"));
    }

}





//================================================
// Remove Connection
//================================================

void WebSocketServer::removeConnection(
    int connectionId)
{

    bool removed = false;
    DisconnectCallback disconnectCallback;



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



    {
        std::lock_guard<std::mutex> lock(
            m_callbackMutex);


        disconnectCallback =
            m_disconnectCallback;
    }


    if(disconnectCallback)
    {
        try
        {
            disconnectCallback(
                connectionId);
        }
        catch(const std::exception& exception)
        {
            std::cerr
                << "Disconnect callback failed: "
                << exception.what()
                << std::endl;
        }
        catch(...)
        {
            std::cerr
                << "Disconnect callback failed"
                << std::endl;
        }

    }

}





//================================================
// Disconnect Callback
//================================================

void WebSocketServer::setDisconnectCallback(
    DisconnectCallback callback)
{

    std::lock_guard<std::mutex> lock(
        m_callbackMutex);

    m_disconnectCallback =
        std::move(callback);

}
