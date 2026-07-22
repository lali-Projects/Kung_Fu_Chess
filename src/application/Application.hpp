#pragma once


#include <memory>
#include <string>





#include "EventBus.hpp"


#include "Board.hpp"
#include "RuleEngine.hpp"
#include "RealTimeArbiter.hpp"
#include "GameEngine.hpp"
#include "GameController.hpp"


#include "GameSession.hpp"

#include "CommandParser.hpp"
#include "CommandHandler.hpp"

#include "Server.hpp"



class Application
{
public:

    Application();

    ~Application();



    Application(const Application&) = delete;
    Application& operator=(const Application&) = delete;



    void start();


    void stop();



    MoveResult sendCommand(
        const std::string& message);



private:

    void initialize();



private:


    /*
        Infrastructure
    */

    std::unique_ptr<EventBus> m_eventBus;



    /*
        Core
    */

    std::unique_ptr<Board> m_board;


    std::unique_ptr<RuleEngine> m_ruleEngine;


    std::unique_ptr<RealTimeArbiter> m_arbiter;


    std::unique_ptr<GameEngine> m_engine;


    std::unique_ptr<GameController> m_controller;



    /*
        Game Layer
    */

    std::unique_ptr<GameSession> m_session;



    /*
        Server Command Layer
    */

    std::unique_ptr<CommandParser> m_commandParser;


    std::unique_ptr<CommandHandler> m_commandHandler;



    /*
        Server Runtime
    */

    std::unique_ptr<Server> m_server;



    bool m_running{false};
};