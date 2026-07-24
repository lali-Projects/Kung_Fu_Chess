#pragma once


#include <memory>
#include <string>


#include "MoveResult.hpp"


class CommandHandler;
class ConnectionManager;
class GameSession;
class EventBus;
class Event;
class INetworkServer;



/**
 * @brief Application server facade.
 *
 * Responsibilities:
 *
 *  - Start server.
 *  - Stop server.
 *  - Own ConnectionManager.
 *  - Handle network input.
 *  - Broadcast game messages.
 *
 *
 * Does NOT know:
 *
 *  - Game rules.
 *  - Board.
 *  - GameEngine.
 *  - Network implementation.
 */
class Server
{

public:


    Server(
        CommandHandler& commandHandler,
        GameSession& session,
        EventBus& eventBus,
        std::unique_ptr<INetworkServer> networkServer);



    ~Server();



    Server(
        const Server&) = delete;


    Server& operator=(
        const Server&) = delete;



public:


    void start();


    void stop();


    bool isRunning() const;



    ConnectionManager&
    getConnectionManager();



    /**
     * @brief Local testing only.
     *
     * Simulates a client command.
     *
     * Same path as real network.
     */
    MoveResult simulateClientCommand(
        const std::string& message);



private:


    void onGameStateChanged(
        std::shared_ptr<Event> event);



private:


    CommandHandler& m_commandHandler;


    GameSession& m_session;


    EventBus& m_eventBus;



    std::unique_ptr<ConnectionManager>
        m_connectionManager;



    std::unique_ptr<INetworkServer>
        m_networkServer;



    bool m_running{false};

};