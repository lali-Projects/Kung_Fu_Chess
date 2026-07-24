#include "Application.hpp"


#include <iostream>


#include "EventBus.hpp"


#include "Board.hpp"
#include "BoardInitializer.hpp"


#include "RuleEngine.hpp"
#include "RealTimeArbiter.hpp"
#include "GameEngine.hpp"
#include "GameController.hpp"


#include "GameSnapshotBuilder.hpp"


#include "GameSession.hpp"
#include "SessionManager.hpp"


#include "CommandHandler.hpp"


#include "ProtocolParser.hpp"


#include "Server.hpp"

#include "ConnectionManager.hpp"
#include "ClientConnection.hpp"



//================================================
// Constructor
//================================================

Application::Application()
{
    initialize();
}



//================================================
// Destructor
//================================================

Application::~Application()
{
    stop();
}



//================================================
// Initialize
//================================================

void Application::initialize()
{

    //---------------------------------
    // Infrastructure
    //---------------------------------

    m_eventBus =
        std::make_unique<EventBus>();



    //---------------------------------
    // Board
    //---------------------------------

    m_board =
        std::make_unique<Board>(
            8,
            8);



    BoardInitializer::setupInitialPosition(
        *m_board);



    //---------------------------------
    // Core
    //---------------------------------

    m_ruleEngine =
        std::make_unique<RuleEngine>();



    m_arbiter =
        std::make_unique<RealTimeArbiter>(
            *m_board);



    m_engine =
        std::make_unique<GameEngine>(
            *m_board,
            *m_ruleEngine,
            *m_arbiter);



    m_controller =
        std::make_unique<GameController>(
            *m_board,
            *m_engine);



    //---------------------------------
    // Snapshot
    //---------------------------------

    m_snapshotBuilder =
        std::make_unique<GameSnapshotBuilder>(
            *m_engine,
            *m_controller);



    //---------------------------------
    // Session
    //---------------------------------

    m_session =
        std::make_unique<GameSession>(
            "game_001",
            std::move(m_controller),
            std::move(m_snapshotBuilder),
            *m_eventBus);



    //---------------------------------
    // Session Manager
    //---------------------------------

    m_sessionManager =
        std::make_unique<SessionManager>();



    m_sessionManager->addSession(
        std::move(m_session));



    //---------------------------------
    // Commands
    //---------------------------------

    m_commandHandler =
        std::make_unique<CommandHandler>(
            *m_sessionManager);



    //---------------------------------
    // Protocol
    //---------------------------------

    m_protocolParser =
        std::make_unique<ProtocolParser>();



    //---------------------------------
    // Server
    //---------------------------------
m_server =
    std::make_unique<Server>(
        *m_commandHandler,
        m_sessionManager->getSession(),
        *m_eventBus);

}



//================================================
// Start
//================================================

void Application::start()
{

    if(m_running)
    {
        return;
    }



    m_running = true;



    m_server->start();



    std::cout
        << "Application started"
        << std::endl;
}



//================================================
// Stop
//================================================

void Application::stop()
{

    if(!m_running)
    {
        return;
    }



    m_server->stop();



    m_running = false;



    std::cout
        << "Application stopped"
        << std::endl;
}



//================================================
// Send Command
//================================================

MoveResult Application::sendCommand(
    const std::string& message)
{

    if(!m_running)
    {
        return
        {
            false,
            "application_not_running"
        };
    }



    /*
        Test client simulation.

        Same path as a real client:

        Application
             |
             v
        ConnectionManager
             |
             v
        ClientConnection
             |
             v
        ConnectionHandler
    */


    ConnectionManager& manager =
        m_server->getConnectionManager();



    int id =
        manager.addConnection();



    ClientConnection* client =
        manager.getConnection(id);



    if(client == nullptr)
    {
        return
        {
            false,
            "client_creation_failed"
        };
    }



    return client->send(
        message);
}



//================================================
// Get Server
//================================================

Server&
Application::getServer()
{
    return *m_server;
}