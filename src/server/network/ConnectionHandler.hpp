#pragma once


#include <string>

#include "MoveResult.hpp"
#include "CommandParser.hpp"



class CommandHandler;
class PlayerSession;


/**
 * @brief Handles messages from one client.
 *
 * Responsibilities:
 *
 *  - Parse raw messages.
 *  - Attach player identity.
 *  - Forward command.
 *
 * Does NOT know:
 *
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



    MoveResult receive(
        const std::string& message);



private:


    CommandParser m_parser;


    CommandHandler& m_commandHandler;


    PlayerSession& m_player;
};