#pragma once


#include "ClickCommand.hpp"
#include "GameSession.hpp"
#include "SessionManager.hpp"
#include "PlayerSession.hpp"



/**
 * @brief Handles commands received from players.
 *
 * Responsibilities:
 *
 *  - Receive parsed commands.
 *  - Forward commands to GameSession.
 *  - Pass player identity.
 *
 *
 * Does NOT know:
 *
 *  - GameEngine.
 *  - Board.
 *  - Rules.
 *  - Network.
 */
class CommandHandler
{
public:


    /**
     * @brief Creates command handler.
     *
     * @param sessionManager
     *        Provides active game session.
     */
    explicit CommandHandler(
        SessionManager& sessionManager);



public:


    /**
     * @brief Handles player click.
     *
     * @param player
     *        Player sending command.
     *
     * @param command
     *        Parsed click command.
     */
    MoveResult handle(
        PlayerSession& player,
        const ClickCommand& command);



private:


    /*
        Provides access
        to active game.
    */
    SessionManager& m_sessionManager;
};