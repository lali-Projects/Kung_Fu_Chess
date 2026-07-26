#pragma once


#include "MoveResult.hpp"



class Command;
class PlayerSession;
class RoomManager;
class AuthService;



/**
 * @brief Routes parsed commands to application services.
 *
 * Responsibilities:
 *
 *  - Receive commands from server layer.
 *  - Route authentication commands.
 *  - Route room commands.
 *  - Route game commands.
 *
 *
 * Does NOT know:
 *
 *  - Network.
 *  - Database.
 *  - Game rules.
 *  - Board.
 *  - GameEngine.
 */
class CommandHandler
{

public:


    CommandHandler(
        RoomManager& roomManager,
        AuthService& authService);



public:


    MoveResult handle(
        PlayerSession* player,
        const Command& command);



private:


    MoveResult handleClick(
        PlayerSession* player,
        const Command& command);



    MoveResult handleLogin(
        const Command& command);



    MoveResult handleRegister(
        const Command& command);



    MoveResult handleCreateRoom(
        const Command& command);



    MoveResult handleJoinRoom(
        PlayerSession* player,
        const Command& command);



private:


    RoomManager& m_roomManager;


    AuthService& m_authService;

};