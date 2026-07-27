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

std::string
PlayerSession::getSessionId() const
{
    std::lock_guard<std::recursive_mutex> lock(m_mutex);
    return m_sessionId;
}






std::string
PlayerSession::getUserId() const
{
    std::lock_guard<std::recursive_mutex> lock(m_mutex);
    return m_userId;
}






std::string
PlayerSession::getUsername() const
{
    std::lock_guard<std::recursive_mutex> lock(m_mutex);
    return m_username;
}








//================================================
// Authentication
//================================================

bool PlayerSession::isAuthenticated() const
{
    std::lock_guard<std::recursive_mutex> lock(m_mutex);
    return m_authenticated;
}






bool PlayerSession::hasUser() const
{
    std::lock_guard<std::recursive_mutex> lock(m_mutex);
    return
        !m_userId.empty();
}






void PlayerSession::authenticate(
    const std::string& userId,
    const std::string& username)
{

    std::lock_guard<std::recursive_mutex> lock(m_mutex);

    m_userId =
        userId;


    m_username =
        username;


    m_authenticated = true;

}






void PlayerSession::logout()
{

    std::lock_guard<std::recursive_mutex> lock(m_mutex);

    m_authenticated = false;


    m_userId.clear();


    m_username.clear();

}









//================================================
// Side
//================================================

Side PlayerSession::getSide() const
{
    std::lock_guard<std::recursive_mutex> lock(m_mutex);
    return m_side;
}






void PlayerSession::setSide(
    Side side)
{
    std::lock_guard<std::recursive_mutex> lock(m_mutex);
    m_side = side;
}






void PlayerSession::clearSide()
{
    std::lock_guard<std::recursive_mutex> lock(m_mutex);
    m_side = Side::NONE;
}









//================================================
// Connection
//================================================

PlayerSession::ConnectionState
PlayerSession::getState() const
{
    std::lock_guard<std::recursive_mutex> lock(m_mutex);
    return m_state;
}






void PlayerSession::setState(
    ConnectionState state)
{
    std::lock_guard<std::recursive_mutex> lock(m_mutex);
    m_state = state;
}






bool PlayerSession::isConnected() const
{
    std::lock_guard<std::recursive_mutex> lock(m_mutex);
    return
        m_state ==
        ConnectionState::CONNECTED;
}






void PlayerSession::disconnect()
{

    std::lock_guard<std::recursive_mutex> lock(m_mutex);

    m_state =
        ConnectionState::DISCONNECTED;

}









//================================================
// Room
//================================================

void PlayerSession::setRoomId(
    const std::string& roomId)
{
    std::lock_guard<std::recursive_mutex> lock(m_mutex);
    m_roomId = roomId;
}






std::string
PlayerSession::getRoomId() const
{
    std::lock_guard<std::recursive_mutex> lock(m_mutex);
    return m_roomId;
}






bool PlayerSession::hasRoom() const
{
    std::lock_guard<std::recursive_mutex> lock(m_mutex);
    return
        !m_roomId.empty();
}






void PlayerSession::clearRoom()
{
    std::lock_guard<std::recursive_mutex> lock(m_mutex);
    m_roomId.clear();
}






void PlayerSession::leaveGame()
{

    std::lock_guard<std::recursive_mutex> lock(m_mutex);

    clearRoom();


    clearSide();

}






bool PlayerSession::isInGame() const
{
    std::lock_guard<std::recursive_mutex> lock(m_mutex);
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
    std::lock_guard<std::recursive_mutex> lock(m_mutex);
    m_connectionId = id;
}






int PlayerSession::getConnectionId() const
{
    std::lock_guard<std::recursive_mutex> lock(m_mutex);
    return m_connectionId;
}









//================================================
// State Helpers
//================================================

bool PlayerSession::canJoinRoom() const
{

    std::lock_guard<std::recursive_mutex> lock(m_mutex);

    return
        isConnected()
        &&
        isAuthenticated()
        &&
        !hasRoom();

}









void PlayerSession::reset()
{

    std::lock_guard<std::recursive_mutex> lock(m_mutex);

    logout();


    leaveGame();


    m_connectionId = -1;


    m_state =
        ConnectionState::CONNECTED;

}
