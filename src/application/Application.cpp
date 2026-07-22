#include "Application.hpp"


#include <iostream>


#include "BoardInitializer.hpp"



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
// Initialize System
//================================================

void Application::initialize()
{

    /*
        Infrastructure
    */

    m_eventBus =
        std::make_unique<EventBus>();



    /*
        Board
    */

    m_board =
        std::make_unique<Board>(
            8,
            8);



    BoardInitializer::setupInitialPosition(
        *m_board);



    /*
        Core Logic
    */

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



    /*
        Controller

        Only gateway into game logic.
    */

    m_controller =
        std::make_unique<GameController>(
            *m_board,
            *m_engine);



    /*
        Session
    */

    m_session =
        std::make_unique<GameSession>(
            "game_001",
            std::move(m_controller),
            *m_eventBus);



    /*
        Command Layer
    */

    m_commandParser =
        std::make_unique<CommandParser>();



    m_commandHandler =
        std::make_unique<CommandHandler>(
            *m_session);



    /*
        Server
    */

    m_server =
        std::make_unique<Server>(
            *m_commandParser,
            *m_commandHandler);
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



    return m_server->handle(
        message);
}