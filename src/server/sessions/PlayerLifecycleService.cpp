#include "PlayerLifecycleService.hpp"

#include "GameSession.hpp"
#include "PlayerSession.hpp"
#include "PlayerSessionManager.hpp"
#include "Room.hpp"
#include "RoomManager.hpp"

PlayerLifecycleService::PlayerLifecycleService(
    RoomManager& roomManager,
    PlayerSessionManager& sessionManager
)
    : m_roomManager(roomManager)
    , m_sessionManager(sessionManager)
{
}

MoveResult PlayerLifecycleService::joinRoom(
    const std::shared_ptr<PlayerSession>& player,
    const std::string& roomId
)
{
    if (!player || !player->isAuthenticated())
    {
        return {false, "authentication_required"};
    }

    if (!player->canJoinRoom())
    {
        return {false, "player_cannot_join_room"};
    }

    auto room = m_roomManager.getRoom(roomId);

    if (!room)
    {
        return {false, "room_not_found"};
    }

    if (!room->canJoin())
    {
        return {false, "room_not_joinable"};
    }

    if (!room->getSession().addPlayer(player))
    {
        return {false, "join_failed"};
    }

    return {true, "joined_room"};
}

MoveResult PlayerLifecycleService::leaveRoom(
    const std::shared_ptr<PlayerSession>& player
)
{
    if (!player || !player->isAuthenticated())
    {
        return {false, "authentication_required"};
    }

    if (!player->hasRoom())
    {
        return {false, "player_has_no_room"};
    }

    if (!removeFromRoom(player))
    {
        return {false, "room_not_found"};
    }

    return {true, "left_room"};
}

MoveResult PlayerLifecycleService::logout(
    const std::shared_ptr<PlayerSession>& player
)
{
    if (!player || !player->isAuthenticated())
    {
        return {false, "authentication_required"};
    }

    removeFromRoom(player);

    const std::string sessionId = player->getSessionId();

    player->logout();
    player->disconnect();
    player->setConnectionId(-1);

    m_sessionManager.removeSession(sessionId);

    return {true, "logout_success"};
}

void PlayerLifecycleService::disconnect(
    const std::shared_ptr<PlayerSession>& player
)
{
    if (!player)
    {
        return;
    }

    removeFromRoom(player);

    const std::string sessionId = player->getSessionId();

    player->disconnect();
    player->setConnectionId(-1);

    m_sessionManager.removeSession(sessionId);
}

bool PlayerLifecycleService::removeFromRoom(
    const std::shared_ptr<PlayerSession>& player
)
{
    if (!player || !player->hasRoom())
    {
        return true;
    }

    const std::string roomId = player->getRoomId();

    auto room = m_roomManager.getRoom(roomId);

    if (!room)
    {
        player->leaveGame();
        return false;
    }

    if (!room->getSession().removePlayer(*player))
    {
        player->leaveGame();
        return false;
    }

    return true;
}