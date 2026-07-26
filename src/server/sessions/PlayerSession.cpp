#include "PlayerSession.hpp"


#include <utility>



//================================================
// Constructor
//================================================

PlayerSession::PlayerSession(
    std::string id)
:
m_id(std::move(id))
{
}



//================================================
// ID
//================================================

const std::string&
PlayerSession::getId() const
{
    return m_id;
}



//================================================
// Side
//================================================

Side PlayerSession::getSide() const
{
    return m_side;
}



void PlayerSession::setSide(
    Side side)
{
    m_side = side;
}



//================================================
// Connection State
//================================================

PlayerSession::ConnectionState
PlayerSession::getState() const
{
    return m_state;
}



void PlayerSession::setState(
    ConnectionState state)
{
    m_state = state;
}



//================================================
// Room
//================================================

void PlayerSession::setRoomId(
    const std::string& roomId)
{
    m_roomId = roomId;
}



const std::string&
PlayerSession::getRoomId() const
{
    return m_roomId;
}



bool PlayerSession::hasRoom() const
{
    return !m_roomId.empty();
}



//================================================
// Connection Id
//================================================

void PlayerSession::setConnectionId(
    int id)
{
    m_connectionId = id;
}



int PlayerSession::getConnectionId() const
{
    return m_connectionId;
}

void PlayerSession::clearRoom()
{
    m_roomId.clear();
}