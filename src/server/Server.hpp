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
class NetworkMessage;



/**
 * @brief Application server facade.
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
     * @brief Access transport layer.
     *
     * Used only for integration tests.
     */
    INetworkServer&
    getNetworkServer();



    /**
     * @brief Development helper.
     *
     * Sends command through same path
     * as real network.
     */
    MoveResult simulateClientCommand(
        const std::string& message);



private:

    void handleNetworkMessage(
        int connectionId,
        const NetworkMessage& message);



    void handleConnection(
        int connectionId);



    void handleDisconnect(
        int connectionId);



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