#include "Server.hpp"


#include "INetworkServer.hpp"
#include "ConnectionManager.hpp"

#include "CommandHandler.hpp"
#include "GameSession.hpp"

#include "ClientConnection.hpp"


#include "EventBus.hpp"
#include "GameStateChangedEvent.hpp"


#include "SnapshotSerializer.hpp"


#include "NetworkMessage.hpp"


#include <iostream>



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


    m_connectionManager =
        std::make_unique<ConnectionManager>(
            m_commandHandler,
            m_session);



    if(m_networkServer)
    {

        m_networkServer->setMessageCallback(
            [this]
            (
                int connectionId,
                const NetworkMessage& message
            )
            {

                ClientConnection* client =
                    m_connectionManager
                        ->getConnection(connectionId);



                if(!client)
                {
                    return;
                }



                client->receiveNetworkMessage(
                    message);
            });
    }



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
// Is Running
//================================================

bool Server::isRunning() const
{
    return m_running;
}



//================================================
// Get Connection Manager
//================================================

ConnectionManager&
Server::getConnectionManager()
{
    return *m_connectionManager;
}



//================================================
// Simulate Client Command
//================================================

MoveResult Server::simulateClientCommand(
    const std::string& message)
{

    int id =
        m_connectionManager->addConnection();



    ClientConnection* client =
        m_connectionManager->getConnection(id);



    if(!client)
    {
        return
        {
            false,
            "client_creation_failed"
        };
    }



    NetworkMessage networkMessage(
        MessageType::COMMAND,
        message);



    return client->send(
        networkMessage);
}



//================================================
// Game State Changed
//================================================

void Server::onGameStateChanged(
    std::shared_ptr<Event> event)
{

    if(!event)
    {
        return;
    }



    auto snapshotEvent =
        std::static_pointer_cast<GameStateChangedEvent>(
            event);



    NetworkMessage message =
        SnapshotSerializer::serialize(
            snapshotEvent->getSnapshot());



    m_connectionManager->broadcast(
        message);
}