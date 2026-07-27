#pragma once


#include <atomic>
#include <memory>
#include <string>


#include "MoveResult.hpp"



class CommandHandler;
class ConnectionManager;
class EventBus;
class Event;
class INetworkServer;
class NetworkMessage;




/**
 * @brief Application server facade.
 *
 * Responsibilities:
 *
 *  - Manage application lifecycle.
 *  - Connect network layer with application layer.
 *  - Route client messages.
 *  - Broadcast game events.
 *
 *
 * Does NOT know:
 *
 *  - Game rules.
 *  - Board.
 *  - Pieces.
 *  - GameEngine.
 *  - Room internals.
 */
class Server
{

public:


    Server(
        CommandHandler& commandHandler,
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





public:


    ConnectionManager&
    getConnectionManager();



    INetworkServer&
    getNetworkServer();





public:


    // Testing

    int createTestConnection();



    void closeTestConnection(
        int connectionId);



    MoveResult simulateClientCommand(
        int connectionId,
        const std::string& message);



    MoveResult simulateClientCommand(
        const std::string& message);





private:


    NetworkMessage handleNetworkMessage(
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



    EventBus& m_eventBus;



    std::unique_ptr<ConnectionManager>
        m_connectionManager;



    std::unique_ptr<INetworkServer>
        m_networkServer;



    std::atomic<bool>
        m_running{false};

};