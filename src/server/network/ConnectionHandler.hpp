#pragma once


#include <string>


#include "MoveResult.hpp"
#include "CommandParser.hpp"



class CommandHandler;
class PlayerSession;




/**
 * @brief Handles commands received from one client.
 *
 * Responsibilities:
 *
 *  - Parse raw messages.
 *  - Convert messages into commands.
 *  - Attach player identity.
 *  - Forward commands.
 *
 *
 * Does NOT know:
 *
 *  - Network transport.
 *  - Game rules.
 *  - Board.
 *  - GameEngine.
 *  - GameSession internals.
 */
class ConnectionHandler
{

public:


    ConnectionHandler(
        CommandHandler& commandHandler,
        PlayerSession& player);



    ~ConnectionHandler() = default;



    ConnectionHandler(
        const ConnectionHandler&) = delete;



    ConnectionHandler& operator=(
        const ConnectionHandler&) = delete;




public:


    /**
     * @brief Receives raw client message.
     *
     * Flow:
     *
     * string
     *   |
     *   v
     * Parser
     *   |
     *   v
     * Command
     *   |
     *   v
     * CommandHandler
     */
    MoveResult receive(
        const std::string& message);




private:


    CommandParser m_parser;



    CommandHandler& m_commandHandler;



    PlayerSession& m_player;

};