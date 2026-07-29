#pragma once

#include <string>

#include "MoveResult.hpp"


class AuthenticationClient;
class AuthoritativeGameState;
class ClientSession;
class RoomClient;

namespace client
{
    class ITransport;
}


class ClientApplicationController
{
public:
    ClientApplicationController(
        client::ITransport& transport,
        AuthenticationClient& authentication,
        RoomClient& rooms,
        ClientSession& session,
        AuthoritativeGameState& gameState);

    MoveResult connectServer(const std::string& url);
    void disconnect();

    MoveResult registerUser(
        const std::string& username,
        const std::string& password);
    MoveResult login(
        const std::string& username,
        const std::string& password);
    MoveResult logout();

    MoveResult createRoom(const std::string& roomId);
    MoveResult joinRoom(const std::string& roomId);
    MoveResult leaveRoom();

    bool isConnected() const;
    bool isAuthenticated() const;
    bool isInRoom() const;

private:
    client::ITransport& m_transport;
    AuthenticationClient& m_authentication;
    RoomClient& m_rooms;
    ClientSession& m_session;
    AuthoritativeGameState& m_gameState;
};
