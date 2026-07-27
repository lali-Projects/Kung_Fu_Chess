#include "PlayerSession.hpp"


#include <utility>





//================================================
// Constructor
//================================================

PlayerSession::PlayerSession(
    std::string sessionId)
:
m_sessionId(std::move(sessionId))
{
}








//================================================
// Identity
//================================================

const std::string&
PlayerSession::getSessionId() const
{
    return m_sessionId;
}






const std::string&
PlayerSession::getUserId() const
{
    return m_userId;
}






const std::string&
PlayerSession::getUsername() const
{
    return m_username;
}








//================================================
// Authentication
//================================================

bool PlayerSession::isAuthenticated() const
{
    return m_authenticated;
}






bool PlayerSession::hasUser() const
{
    return
        !m_userId.empty();
}






void PlayerSession::authenticate(
    const std::string& userId,
    const std::string& username)
{

    m_userId =
        userId;


    m_username =
        username;


    m_authenticated = true;

}






void PlayerSession::logout()
{

    m_authenticated = false;


    m_userId.clear();


    m_username.clear();

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






void PlayerSession::clearSide()
{
    m_side = Side::NONE;
}









//================================================
// Connection
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






bool PlayerSession::isConnected() const
{
    return
        m_state ==
        ConnectionState::CONNECTED;
}






void PlayerSession::disconnect()
{

    m_state =
        ConnectionState::DISCONNECTED;

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
    return
        !m_roomId.empty();
}






void PlayerSession::clearRoom()
{
    m_roomId.clear();
}






void PlayerSession::leaveGame()
{

    clearRoom();


    clearSide();

}






bool PlayerSession::isInGame() const
{
    return
        hasRoom()
        &&
        m_side != Side::NONE;
}









//================================================
// Connection ID
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









//================================================
// State Helpers
//================================================

bool PlayerSession::canJoinRoom() const
{

    return
        isConnected()
        &&
        isAuthenticated()
        &&
        !hasRoom();

}









void PlayerSession::reset()
{

    logout();


    leaveGame();


    m_connectionId = -1;


    m_state =
        ConnectionState::CONNECTED;

}