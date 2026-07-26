#include "Server.hpp"


#include <iostream>


#include "INetworkServer.hpp"
#include "ConnectionManager.hpp"
#include "ClientConnection.hpp"

#include "EventBus.hpp"
#include "GameStateChangedEvent.hpp"

#include "SnapshotSerializer.hpp"
#include "NetworkMessage.hpp"



//================================================
// Constructor
//================================================

Server::Server(
    CommandHandler& commandHandler,
    GameSession& session,
    EventBus& eventBus,
    std::unique_ptr<INetworkServer> networkServer)
:
m_commandHandler(commandHandler),
m_session(session),
m_eventBus(eventBus),
m_networkServer(std::move(networkServer))
{

    //---------------------------------
    // Connection Manager
    //---------------------------------

    m_connectionManager =
        std::make_unique<ConnectionManager>(
            m_commandHandler,
            m_session);



    //---------------------------------
    // Outgoing messages flow
    //
    // Game
    //  |
    // EventBus
    //  |
    // Server
    //  |
    // ConnectionManager
    //  |
    // INetworkServer
    //---------------------------------

    m_connectionManager->setSendCallback(
        [this]
        (
            int connectionId,
            const NetworkMessage& message
        )
        {

            if(m_networkServer)
            {
                m_networkServer->send(
                    connectionId,
                    message);
            }

        });



    //---------------------------------
    // Network callbacks
    //---------------------------------

    if(m_networkServer)
    {

        m_networkServer->setConnectionCallback(
            [this]
            (
                int id
            )
            {
                handleConnection(id);
            });



        m_networkServer->setMessageCallback(
            [this]
            (
                int id,
                const NetworkMessage& message
            )
            {
                handleNetworkMessage(
                    id,
                    message);
            });



        m_networkServer->setDisconnectCallback(
            [this]
            (
                int id
            )
            {
                handleDisconnect(id);
            });

    }



    //---------------------------------
    // Game state updates
    //---------------------------------

    m_eventBus.subscribe<GameStateChangedEvent>(
        [this]
        (
            std::shared_ptr<GameStateChangedEvent> event
        )
        {
            onGameStateChanged(event);
        });

}



//================================================
// Destructor
//================================================

Server::~Server()
{
    stop();
}



//================================================
// Start
//================================================

void Server::start()
{

    if(m_running)
    {
        return;
    }



    if(m_networkServer)
    {
        m_networkServer->start();
    }



    m_running = true;



    std::cout
        << "Server started"
        << std::endl;
}



//================================================
// Stop
//================================================

void Server::stop()
{

    if(!m_running)
    {
        return;
    }



    if(m_networkServer)
    {
        m_networkServer->stop();
    }



    m_running = false;



    std::cout
        << "Server stopped"
        << std::endl;
}



//================================================
// Running
//================================================

bool Server::isRunning() const
{
    return m_running;
}



//================================================
// Connection Manager
//================================================

ConnectionManager&
Server::getConnectionManager()
{
    return *m_connectionManager;
}



//================================================
// Network Server
//================================================

INetworkServer&
Server::getNetworkServer()
{
    return *m_networkServer;
}



//================================================
// Connection Created
//================================================

void Server::handleConnection(
    int connectionId)
{

    bool added =
        m_connectionManager->addConnection(
            connectionId);



    if(added)
    {

        std::cout
            << "Client registered: "
            << connectionId
            << std::endl;
    }

}



//================================================
// Disconnect
//================================================

void Server::handleDisconnect(
    int connectionId)
{

    m_connectionManager->removeConnection(
        connectionId);



    std::cout
        << "Client removed: "
        << connectionId
        << std::endl;
}



//================================================
// Incoming Network Message
//================================================

//================================================
// Incoming Network Message
//================================================

void Server::handleNetworkMessage(
    int connectionId,
    const NetworkMessage& message)
{
std::cout
    << "[SERVER] received network message: "
    << message.getPayload()
    << std::endl;
    ClientConnection* client =
        m_connectionManager->getConnection(
            connectionId);



    if(!client)
    {
        return;
    }



    MoveResult result =
        client->receiveNetworkMessage(
            message);



    std::cout
        << "[SERVER RESULT] "
        << result.success
        << " "
        << result.reason
        << std::endl;



    //---------------------------------
    // Send command result back
    //---------------------------------

    std::string responseText;



    if(result.success)
    {
        responseText =
            "RESULT SUCCESS "
            +
            result.reason;
    }
    else
    {
        responseText =
            "RESULT FAILED "
            +
            result.reason;
    }



    NetworkMessage response(
        MessageType::COMMAND_RESULT,
        responseText);



    client->deliverMessage(
        response);

}



//================================================
// Local Test Command
//================================================

MoveResult Server::simulateClientCommand(
    const std::string& message)
{

    int id =
        m_connectionManager->addConnection();



    if(id < 0)
    {
        return
        {
            false,
            "connection_failed"
        };
    }



    ClientConnection* client =
        m_connectionManager->getConnection(id);



    if(!client)
    {
        return
        {
            false,
            "client_missing"
        };
    }



    NetworkMessage networkMessage(
        MessageType::COMMAND,
        message);



    return client->receiveNetworkMessage(
        networkMessage);

}



//================================================
// Snapshot Broadcast
//================================================

void Server::onGameStateChanged(
    std::shared_ptr<Event> event)
{
    std::cout
        << "[SERVER] onGameStateChanged BEGIN"
        << std::endl;


    if(!event)
    {
        std::cout
            << "[SERVER] EVENT NULL"
            << std::endl;

        return;
    }


    std::cout
        << "[SERVER] casting event"
        << std::endl;


    auto snapshotEvent =
        std::static_pointer_cast<GameStateChangedEvent>(
            event);


    std::cout
        << "[SERVER] event cast success"
        << std::endl;



    NetworkMessage message =
        SnapshotSerializer::serialize(
            snapshotEvent->getSnapshot());


    std::cout
        << "[SERVER] snapshot serialized"
        << std::endl;



    std::cout
        << "[SERVER] before broadcast"
        << std::endl;


    m_connectionManager->broadcast(
        message);


    std::cout
        << "[SERVER] after broadcast"
        << std::endl;
}