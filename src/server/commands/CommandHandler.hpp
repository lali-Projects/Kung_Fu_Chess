#pragma once


#include "MoveResult.hpp"



class ClickCommand;

class SessionManager;
class PlayerSession;
class GameSession;



/**
 * @brief Dispatches validated commands to the game session.
 *
 *
 * Responsibilities:
 *
 *  - Receive validated commands.
 *  - Keep player context.
 *  - Forward commands to GameSession.
 *
 *
 * Does NOT know:
 *
 *  - NetworkMessage.
 *  - JSON.
 *  - WebSocket.
 *  - Protocol parsing.
 *  - Game rules.
 *  - Board.
 *  - GameEngine.
 */
class CommandHandler
{
public:


    explicit CommandHandler(
        SessionManager& sessionManager);



public:


    /**
     * @brief Handles a click command.
     *
     * The command is already parsed
     * and structurally valid.
     */
    MoveResult handle(
        PlayerSession& player,
        const ClickCommand& command);



private:


    SessionManager& m_sessionManager;
};