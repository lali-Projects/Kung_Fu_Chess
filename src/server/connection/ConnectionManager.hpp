#pragma once


#include <map>
#include <memory>


#include "NetworkMessage.hpp"



class ClientConnection;
class CommandHandler;
class GameSession;



/**
 * @brief Manages connected clients.
 *
 * Responsibilities:
 *
 *  - Create client connections.
 *  - Store active connections.
 *  - Remove disconnected clients.
 *  - Broadcast network messages.
 *
 *
 * Does NOT know:
 *
 *  - Game rules.
 *  - Board.
 *  - GameEngine.
 *  - GameSnapshot.
 *  - Serialization.
 */
class ConnectionManager
{

public:


    explicit ConnectionManager(
        CommandHandler& commandHandler,
        GameSession& session);



    ~ConnectionManager();



    ConnectionManager(
        const ConnectionManager&) = delete;


    ConnectionManager& operator=(
        const ConnectionManager&) = delete;



public:


    /**
     * @brief Creates new client connection.
     */
    int addConnection();



    /**
     * @brief Removes client connection.
     */
    void removeConnection(
        int id);



    /**
     * @brief Finds client connection.
     */
    ClientConnection* getConnection(
        int id);



    /**
     * @brief Sends message to all clients.
     */
    void broadcast(
        const NetworkMessage& message);



    /**
     * @brief Number of active clients.
     */
    size_t size() const;



private:


    int m_nextId{1};



    /*
        Provides access to
        active game session.

        Used only for player registration.
    */
    GameSession& m_session;



    /*
        Dispatches commands
        received from clients.
    */
    CommandHandler& m_commandHandler;



    std::map<
        int,
        std::unique_ptr<ClientConnection>>
        m_connections;

};