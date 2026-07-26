#pragma once


#include <string>


#include "MoveResult.hpp"
#include "CommandParser.hpp"



class CommandHandler;
class PlayerSession;



/**
 * @brief Processes commands from one client connection.
 *
 * Responsibilities:
 *
 *  - Validate raw messages.
 *  - Parse protocol commands.
 *  - Forward commands to CommandHandler.
 *
 *
 * Does NOT know:
 *
 *  - Network transport.
 *  - Database.
 *  - Authentication logic.
 *  - Rooms.
 *  - Game rules.
 *  - GameEngine.
 */
class ConnectionHandler
{

public:


    explicit ConnectionHandler(
        CommandHandler& commandHandler);



    ~ConnectionHandler() = default;



    ConnectionHandler(
        const ConnectionHandler&) = delete;



    ConnectionHandler& operator=(
        const ConnectionHandler&) = delete;



public:


    MoveResult receive(
        const std::string& message,
        PlayerSession* player = nullptr);



private:


    CommandParser m_parser;


    CommandHandler& m_commandHandler;

};