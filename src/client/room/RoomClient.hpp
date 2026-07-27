#pragma once

#include <string>

#include "MoveResult.hpp"


class ClientSession;

namespace client
{
    class IClientCommandGateway;
}


/** Client-side room command service. */
class RoomClient
{
public:
    RoomClient(
        client::IClientCommandGateway& commandGateway,
        ClientSession& session);

    MoveResult createRoom(const std::string& roomId);
    MoveResult joinRoom(const std::string& roomId);
    MoveResult leaveRoom();

private:
    client::IClientCommandGateway& m_commandGateway;
    ClientSession& m_session;
};
