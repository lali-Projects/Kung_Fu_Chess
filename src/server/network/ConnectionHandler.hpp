#pragma once

#include <string>

#include "CommandParser.hpp"
#include "CommandHandler.hpp"


/**
 * @brief Handles communication with one client connection.
 *
 * This is an abstraction layer between:
 *
 * External communication
 *          |
 *          v
 * ConnectionHandler
 *          |
 *          v
 * Command system
 *          |
 *          v
 * Game logic
 *
 *
 * Responsibilities:
 *
 *  - Receive raw client messages.
 *  - Parse commands.
 *  - Forward commands to CommandHandler.
 *
 *
 * Does NOT know:
 *
 *  - GameEngine.
 *  - Board.
 *  - Game rules.
 *  - MOVE/JUMP logic.
 */
class ConnectionHandler
{
public:

    /**
     * @brief Creates a connection handler.
     *
     * @param commandHandler Responsible for executing commands.
     */
    explicit ConnectionHandler(
        CommandHandler& commandHandler);



    /**
     * @brief Processes incoming client message.
     *
     * Example:
     *
     * "CLICK 3 4"
     *
     * @return Result from game logic.
     */
    MoveResult receive(
        const std::string& message);



private:

    /**
     * @brief Parses client messages.
     */
    CommandParser m_parser;



    /**
     * @brief Executes parsed commands.
     */
    CommandHandler& m_commandHandler;
};