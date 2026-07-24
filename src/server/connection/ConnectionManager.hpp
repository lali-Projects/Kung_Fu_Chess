#pragma once


#include <map>
#include <memory>


#include "GameSnapshot.hpp"



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
 *  - Broadcast snapshots.
 *
 *
 * Does NOT know:
 *
 *  - Game rules.
 *  - Board.
 *  - GameEngine.
 *  - GameSession.
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
     * @brief Creates new client.
     */
    int addConnection();



    /**
     * @brief Removes client.
     */
    void removeConnection(
        int id);



    /**
     * @brief Finds client.
     */
    ClientConnection* getConnection(
        int id);



    /**
     * @brief Sends snapshot to all clients.
     */
    void broadcastSnapshot(
        const GameSnapshot& snapshot);



    /**
     * @brief Number of clients.
     */
    size_t size() const;


private:


    int m_nextId{1};

GameSession& m_session;

    CommandHandler& m_commandHandler;



    std::map<
        int,
        std::unique_ptr<ClientConnection>>
        m_connections;

};