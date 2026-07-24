#pragma once


#include <memory>
#include <string>
#include <optional>


#include "MoveResult.hpp"
#include "GameSnapshot.hpp"



class ConnectionHandler;
class CommandHandler;
class PlayerSession;



/**
 * @brief Represents one connected client.
 *
 * Responsibilities:
 *
 *  - Own player identity.
 *  - Receive commands from client.
 *  - Forward commands through ConnectionHandler.
 *  - Receive game snapshots from server.
 *
 *
 * Does NOT know:
 *
 *  - GameEngine.
 *  - Board.
 *  - Rules.
 *  - GameSession.
 */
class ClientConnection
{

public:


    /**
     * @brief Creates client connection.
     *
     * @param id
     *        Unique connection identifier.
     *
     * @param commandHandler
     *        Command execution layer.
     */
    ClientConnection(
        int id,
        CommandHandler& commandHandler);



    ~ClientConnection();



    ClientConnection(
        const ClientConnection&) = delete;


    ClientConnection& operator=(
        const ClientConnection&) = delete;



public:


    /**
     * @brief Receives command from client.
     *
     * Flow:
     *
     * Client
     *   |
     *   v
     * ConnectionHandler
     *   |
     *   v
     * CommandHandler
     */
    MoveResult send(
        const std::string& message);



    /**
     * @brief Receives updated game state.
     *
     * Called by Server broadcast.
     *
     * ClientConnection only stores
     * the information.
     *
     * It does not process game logic.
     */
    void deliverSnapshot(
        const GameSnapshot& snapshot);



    /**
     * @brief Returns last received snapshot.
     *
     * Used by:
     *
     * - Tests.
     * - Future network layer.
     * - GUI client simulation.
     */
    const std::optional<GameSnapshot>&
    getLastSnapshot() const;



    /**
     * @brief Returns connection id.
     */
    int getId() const;



    /**
     * @brief Returns player identity.
     *
     * Shared with GameSession.
     */
    std::shared_ptr<PlayerSession>
    getPlayer();



    /**
     * @brief Returns player reference.
     */
    PlayerSession&
    getPlayerSession();



private:


    //---------------------------------
    // Connection identity
    //---------------------------------

    int m_id;



    //---------------------------------
    // Player identity
    //---------------------------------

    std::shared_ptr<PlayerSession>
        m_player;



    //---------------------------------
    // Command communication
    //---------------------------------

    std::unique_ptr<ConnectionHandler>
        m_handler;



    //---------------------------------
    // Last received game state
    //---------------------------------

    std::optional<GameSnapshot>
        m_lastSnapshot;

};