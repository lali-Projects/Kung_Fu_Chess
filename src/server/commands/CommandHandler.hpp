#pragma once


#include "MoveResult.hpp"



class ClickCommand;
class PlayerSession;
class RoomManager;




/**
 * @brief Dispatches commands to the correct room.
 *
 * Responsibilities:
 *
 *  - Receive commands.
 *  - Locate player's room.
 *  - Forward command to GameSession.
 *
 *
 * Does NOT know:
 *
 *  - Network.
 *  - Rules.
 *  - Board.
 *  - GameEngine.
 */
class CommandHandler
{

public:


    explicit CommandHandler(
        RoomManager& roomManager);



public:


    MoveResult handle(
        PlayerSession& player,
        const ClickCommand& command);



private:


    RoomManager& m_roomManager;

};