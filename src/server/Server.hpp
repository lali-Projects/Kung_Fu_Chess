#pragma once


#include <memory>


class CommandHandler;
class ConnectionManager;
class GameSession;
class EventBus;
class Event;



/**
 * @brief Application server facade.
 *
 * Responsibilities:
 *
 *  - Start server.
 *  - Stop server.
 *  - Own ConnectionManager.
 *  - Listen for game state events.
 *  - Forward snapshots to ConnectionManager.
 *
 *
 * Does NOT know:
 *
 *  - Game rules.
 *  - Board.
 *  - GameEngine.
 *  - ClientConnection objects.
 */
class Server
{

public:


    Server(
        CommandHandler& commandHandler,
        GameSession& session,
        EventBus& eventBus);



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



private:


    /**
     * @brief Receives game state changes.
     */
    void onGameStateChanged(
        std::shared_ptr<Event> event);



private:


    CommandHandler& m_commandHandler;


    GameSession& m_session;


    EventBus& m_eventBus;



    std::unique_ptr<ConnectionManager>
        m_connectionManager;



    bool m_running{false};

};