#include "Server.hpp"


#include "CommandHandler.hpp"
#include "ConnectionManager.hpp"

#include "GameSession.hpp"

#include "EventBus.hpp"
#include "GameStateChangedEvent.hpp"

#include <iostream>



//================================================
// Constructor
//================================================

Server::Server(
    CommandHandler& commandHandler,
    GameSession& session,
    EventBus& eventBus)
:
m_commandHandler(commandHandler),
m_session(session),
m_eventBus(eventBus)
{

    m_connectionManager =
        std::make_unique<ConnectionManager>(
            m_commandHandler,
            m_session);



    /*
        Server observes game state changes.

        Flow:

        GameSession
             |
             v
        EventBus
             |
             v
        Server
             |
             v
        ConnectionManager
             |
             v
        Clients
    */

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



    m_connectionManager->broadcastSnapshot(
        snapshotEvent->getSnapshot());
}