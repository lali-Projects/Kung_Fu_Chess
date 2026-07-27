#pragma once


#include "MoveResult.hpp"



class Command;
class RoomManager;
class AuthService;
class ClientConnection;



class CommandHandler
{

public:


    CommandHandler(
        RoomManager& roomManager,
        AuthService& authService);



public:


    MoveResult handle(
        ClientConnection& connection,
        const Command& command);




private:


    MoveResult handleClick(
        ClientConnection& connection,
        const Command& command);



    MoveResult handleLogin(
        ClientConnection& connection,
        const Command& command);



    MoveResult handleRegister(
        const Command& command);



    MoveResult handleCreateRoom(
        ClientConnection& connection,
        const Command& command);



    MoveResult handleJoinRoom(
        ClientConnection& connection,
        const Command& command);




private:


    RoomManager& m_roomManager;


    AuthService& m_authService;

};