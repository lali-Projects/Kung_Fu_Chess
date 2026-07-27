#include "RoomClient.hpp"

#include "ClientSession.hpp"
#include "CommandResponse.hpp"
#include "IClientCommandGateway.hpp"


RoomClient::RoomClient(
    client::IClientCommandGateway& commandGateway,
    ClientSession& session)
    : m_commandGateway(commandGateway),
      m_session(session)
{
}


MoveResult RoomClient::createRoom(
    const std::string& roomId)
{
    const CommandResponse response =
        m_commandGateway.sendCommandAndWait(
            "CREATE_ROOM " + roomId);
    return {response.success, response.reason};
}


MoveResult RoomClient::joinRoom(
    const std::string& roomId)
{
    const CommandResponse response =
        m_commandGateway.sendCommandAndWait(
            "JOIN_ROOM " + roomId);

    const bool validSide =
        response.side &&
        (*response.side == Side::WHITE ||
         *response.side == Side::BLACK ||
         *response.side == Side::OBSERVER);

    if(response.success &&
       response.roomId &&
       validSide)
    {
        m_session.setRoomId(*response.roomId);
        m_session.setSide(*response.side);
    }

    if(response.success &&
       (!response.roomId || !validSide))
    {
        return {false, "invalid_join_response"};
    }

    return {response.success, response.reason};
}


MoveResult RoomClient::leaveRoom()
{
    const CommandResponse response =
        m_commandGateway.sendCommandAndWait(
            "LEAVE_ROOM");

    if(response.success)
    {
        m_session.clearRoom();
    }

    return {response.success, response.reason};
}
