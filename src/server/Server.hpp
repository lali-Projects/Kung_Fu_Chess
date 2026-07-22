#pragma once


#include <string>


#include "CommandParser.hpp"
#include "CommandHandler.hpp"



/**
 * @brief Main server entry point.
 *
 * Responsibilities:
 *
 *  - Manage server lifecycle.
 *  - Receive raw client messages.
 *  - Parse commands.
 *  - Forward commands to CommandHandler.
 *
 *
 * Does NOT know:
 *
 *  - Game rules.
 *  - Board.
 *  - GameEngine.
 *  - Player logic.
 *  - Command execution details.
 */
class Server
{
public:

    /**
     * @brief Creates server.
     *
     * @param commandParser
     *        Converts raw text into commands.
     *
     * @param commandHandler
     *        Executes parsed commands.
     */
    Server(
        CommandParser& commandParser,
        CommandHandler& commandHandler);



    /**
     * @brief Stops server automatically.
     */
    ~Server();



    Server(const Server&) = delete;
    Server& operator=(const Server&) = delete;



    /**
     * @brief Starts server runtime.
     */
    void start();



    /**
     * @brief Stops server runtime.
     */
    void stop();



    /**
     * @brief Checks server state.
     */
    bool isRunning() const;



    /**
     * @brief Handles raw client message.
     *
     * Example:
     *
     * "CLICK 6 0"
     *
     * Flow:
     *
     * Message
     *    |
     *    v
     * CommandParser
     *    |
     *    v
     * CommandHandler
     */
    MoveResult handle(
        const std::string& message);



private:

    /*
        Converts text commands
        into command objects.
    */
    CommandParser& m_commandParser;



    /*
        Executes commands
        against the game session.
    */
    CommandHandler& m_commandHandler;



    /*
        Server runtime state.
    */
    bool m_running{false};
};