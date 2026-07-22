#include "PlayerSession.hpp"


//================================================
// Constructor
//================================================

PlayerSession::PlayerSession(
    std::string id)
    :
    m_id(std::move(id)),
    m_side(Side::WHITE),
    m_state(ConnectionState::CONNECTED)
{
}


//================================================
// Get ID
//================================================

const std::string& PlayerSession::getId() const
{
    return m_id;
}


//================================================
// Get Side
//================================================

Side PlayerSession::getSide() const
{
    return m_side;
}


//================================================
// Set Side
//================================================

void PlayerSession::setSide(
    Side side)
{
    m_side = side;
}


//================================================
// Get Connection State
//================================================

PlayerSession::ConnectionState
PlayerSession::getState() const
{
    return m_state;
}


//================================================
// Set Connection State
//================================================

void PlayerSession::setState(
    ConnectionState state)
{
    m_state = state;
}