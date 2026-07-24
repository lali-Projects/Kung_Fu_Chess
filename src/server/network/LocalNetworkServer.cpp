#include "LocalNetworkServer.hpp"


#include <iostream>
#include <utility>



//================================================
// Constructor
//================================================

LocalNetworkServer::LocalNetworkServer()
{
}



//================================================
// Start
//================================================

void LocalNetworkServer::start()
{

    if(m_running)
    {
        return;
    }



    m_running = true;



    std::cout
        << "LocalNetworkServer started"
        << std::endl;
}



//================================================
// Stop
//================================================

void LocalNetworkServer::stop()
{

    if(!m_running)
    {
        return;
    }



    m_running = false;



    m_messageCallback = nullptr;



    std::cout
        << "LocalNetworkServer stopped"
        << std::endl;
}



//================================================
// Is Running
//================================================

bool LocalNetworkServer::isRunning() const
{
    return m_running;
}



//================================================
// Send
//================================================

void LocalNetworkServer::send(
    int connectionId,
    const NetworkMessage& message)
{

    if(!m_running)
    {
        return;
    }



    std::cout
        << "[Send] Client "
        << connectionId
        << " <- Type: "
        << static_cast<int>(
            message.getType())
        << " Payload: "
        << message.getPayload()
        << std::endl;
}



//================================================
// Disconnect
//================================================

void LocalNetworkServer::disconnect(
    int connectionId)
{

    if(!m_running)
    {
        return;
    }



    std::cout
        << "[Disconnect] Client "
        << connectionId
        << std::endl;
}



//================================================
// Set Callback
//================================================

void LocalNetworkServer::setMessageCallback(
    MessageCallback callback)
{

    m_messageCallback =
        std::move(callback);
}



//================================================
// Simulate Incoming Message
//================================================

void LocalNetworkServer::simulateIncomingMessage(
    int connectionId,
    const NetworkMessage& message)
{

    if(!m_running)
    {
        return;
    }



    if(!m_messageCallback)
    {
        return;
    }



    m_messageCallback(
        connectionId,
        message);
}