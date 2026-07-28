#pragma once

#include "CommandResponse.hpp"
#include "MoveResult.hpp"

class Command;
class RoomManager;
class AuthService;
class ClientConnection;
class PlayerLifecycleService;

/**
 * @brief Dispatches parsed client commands to the appropriate server services.
 *
 * Handles authentication, room management, gameplay clicks, and player lifecycle
 * operations, and builds the command response returned to the client.
 */

class CommandHandler
{
public:
    CommandHandler(RoomManager& roomManager, AuthService& authService, PlayerLifecycleService& playerLifecycle);

    CommandResponse handle(ClientConnection& connection, const Command& command);

private:
    MoveResult handleClick(ClientConnection& connection, const Command& command);
    MoveResult handleLogin(ClientConnection& connection, const Command& command);
    MoveResult handleRegister(const Command& command);
    MoveResult handleCreateRoom(ClientConnection& connection, const Command& command);
    MoveResult handleJoinRoom(ClientConnection& connection, const Command& command);
    MoveResult handleLeaveRoom(ClientConnection& connection);
    MoveResult handleLogout(ClientConnection& connection);

private:
    RoomManager& m_roomManager;
    AuthService& m_authService;
    PlayerLifecycleService& m_playerLifecycle;
};