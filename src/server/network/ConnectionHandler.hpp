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
 *  - Validate incoming message.
 *  - Parse raw command.
 *  - Forward command with player identity.
 *
 *
 * Does NOT know:
 *
 *  - Network.
 *  - Rooms.
 *  - Game rules.
 *  - Board.
 *  - GameEngine.
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


    MoveResult receive(
        const std::string& message);



private:


    CommandParser m_parser;



    CommandHandler& m_commandHandler;



    PlayerSession& m_player;

};