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

    m_connectionCallback = nullptr;

    m_disconnectCallback = nullptr;



    m_sentMessages.clear();



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



    m_sentMessages[connectionId] =
        message;



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



    if(m_disconnectCallback)
    {
        m_disconnectCallback(
            connectionId);
    }
}



//================================================
// Set Message Callback
//================================================

void LocalNetworkServer::setMessageCallback(
    MessageCallback callback)
{

    m_messageCallback =
        std::move(callback);
}



//================================================
// Set Connection Callback
//================================================

void LocalNetworkServer::setConnectionCallback(
    ConnectionCallback callback)
{

    m_connectionCallback =
        std::move(callback);
}



//================================================
// Simulate New Connection
//================================================

int LocalNetworkServer::simulateNewConnection()
{

    if(!m_running)
    {
        return -1;
    }



    int connectionId =
        m_nextConnectionId++;



    if(m_connectionCallback)
    {
        m_connectionCallback(
            connectionId);
    }



    return connectionId;
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

void LocalNetworkServer::setDisconnectCallback(
    DisconnectCallback callback)
{

    m_disconnectCallback =
        std::move(callback);

}

//================================================
// Get Last Sent Message
//================================================

std::optional<NetworkMessage>
LocalNetworkServer::getLastSentMessage(
    int connectionId) const
{

    auto iterator =
        m_sentMessages.find(
            connectionId);



    if(iterator ==
       m_sentMessages.end())
    {
        return std::nullopt;
    }



    return iterator->second;
}




//================================================
// Clear Messages
//================================================

void LocalNetworkServer::clearSentMessages()
{
    m_sentMessages.clear();
}