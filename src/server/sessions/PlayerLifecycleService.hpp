#pragma once


#include <memory>
#include <string>


#include "MoveResult.hpp"


class PlayerSession;
class PlayerSessionManager;
class RoomManager;


/**
 * Coordinates player membership cleanup across the session and room
 * registries. Network classes notify this service, but remain unaware of
 * room internals.
 */
class PlayerLifecycleService
{
public:
    PlayerLifecycleService(
        RoomManager& roomManager,
        PlayerSessionManager& sessionManager);


    MoveResult joinRoom(
        const std::shared_ptr<PlayerSession>& player,
        const std::string& roomId);


    MoveResult leaveRoom(
        const std::shared_ptr<PlayerSession>& player);


    MoveResult logout(
        const std::shared_ptr<PlayerSession>& player);


    void disconnect(
        const std::shared_ptr<PlayerSession>& player);


private:
    bool removeFromRoom(
        const std::shared_ptr<PlayerSession>& player);


private:
    RoomManager& m_roomManager;
    PlayerSessionManager& m_sessionManager;
};
