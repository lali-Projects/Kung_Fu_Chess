#pragma once

#include "ClickCommand.hpp"
#include "GameSession.hpp"


/**
 * @brief Handles incoming game commands.
 *
 * Converts external commands into
 * calls on GameSession.
 *
 * Flow:
 *
 * ClickCommand
 *       |
 *       v
 * CommandHandler
 *       |
 *       v
 * GameSession
 *
 *
 * Does NOT know:
 *  - GameEngine
 *  - Board
 *  - Rules
 */
class CommandHandler
{
public:

    /**
     * @brief Creates command handler.
     *
     * @param session Active game session.
     */
    explicit CommandHandler(
        GameSession& session);



    /**
     * @brief Handles click command.
     */
    MoveResult handle(
        const ClickCommand& command);



private:

    /**
     * @brief Game controlled by this handler.
     */
    GameSession& m_session;
};