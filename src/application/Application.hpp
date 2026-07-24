#pragma once

#include <memory>
#include <string>

#include "MoveResult.hpp"


class EventBus;

class Board;
class RuleEngine;
class RealTimeArbiter;
class GameEngine;
class GameController;

class GameSnapshotBuilder;

class GameSession;
class SessionManager;

class CommandHandler;

class ProtocolParser;

class Server;

class INetworkServer;



/**
 * @brief Composition root of the application.
 *
 * Creates all objects and connects dependencies.
 *
 * Responsibilities:
 *
 *  - Object creation.
 *  - Dependency injection.
 *  - Application lifecycle.
 *
 *
 * Does NOT know:
 *
 *  - Game rules execution.
 *  - Client connections.
 *  - Network behavior.
 */
class Application
{

public:

    Application();

    ~Application();


    Application(
        const Application&) = delete;


    Application& operator=(
        const Application&) = delete;



public:

    void start();


    void stop();



    MoveResult sendCommand(
        const std::string& message);



    Server& getServer();



private:

    void initialize();



private:


    std::unique_ptr<EventBus>
        m_eventBus;



    std::unique_ptr<Board>
        m_board;


    std::unique_ptr<RuleEngine>
        m_ruleEngine;


    std::unique_ptr<RealTimeArbiter>
        m_arbiter;


    std::unique_ptr<GameEngine>
        m_engine;


    std::unique_ptr<GameController>
        m_controller;



    std::unique_ptr<GameSnapshotBuilder>
        m_snapshotBuilder;



    std::unique_ptr<GameSession>
        m_session;


    std::unique_ptr<SessionManager>
        m_sessionManager;



    std::unique_ptr<CommandHandler>
        m_commandHandler;



    std::unique_ptr<ProtocolParser>
        m_protocolParser;



    std::unique_ptr<Server>
        m_server;



    bool m_running{false};

};