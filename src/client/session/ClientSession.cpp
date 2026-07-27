#include "ClientSession.hpp"


void ClientSession::setConnected(bool connected)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    m_connected = connected;
    if(!connected)
    {
        clearAuthenticatedStateUnsafe();
    }
}


bool ClientSession::isConnected() const
{
    std::lock_guard<std::mutex> lock(m_mutex);
    return m_connected;
}


void ClientSession::login(
    const std::string& userId,
    const std::string& username,
    const std::string& sessionId)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    m_userId = userId;
    m_username = username;
    m_sessionId = sessionId;
    m_authenticated = true;
}


void ClientSession::logout()
{
    std::lock_guard<std::mutex> lock(m_mutex);
    clearAuthenticatedStateUnsafe();
}


bool ClientSession::isAuthenticated() const
{
    std::lock_guard<std::mutex> lock(m_mutex);
    return m_authenticated;
}


std::string ClientSession::getUserId() const
{
    std::lock_guard<std::mutex> lock(m_mutex);
    return m_userId;
}


std::string ClientSession::getUsername() const
{
    std::lock_guard<std::mutex> lock(m_mutex);
    return m_username;
}


std::string ClientSession::getSessionId() const
{
    std::lock_guard<std::mutex> lock(m_mutex);
    return m_sessionId;
}


void ClientSession::setRoomId(const std::string& roomId)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    m_roomId = roomId;
}


void ClientSession::clearRoom()
{
    std::lock_guard<std::mutex> lock(m_mutex);
    m_roomId.clear();
    m_side = Side::NONE;
}


bool ClientSession::hasRoom() const
{
    std::lock_guard<std::mutex> lock(m_mutex);
    return !m_roomId.empty();
}


std::string ClientSession::getRoomId() const
{
    std::lock_guard<std::mutex> lock(m_mutex);
    return m_roomId;
}


void ClientSession::setSide(Side side)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    m_side = side;
}


void ClientSession::clearSide()
{
    std::lock_guard<std::mutex> lock(m_mutex);
    m_side = Side::NONE;
}


Side ClientSession::getSide() const
{
    std::lock_guard<std::mutex> lock(m_mutex);
    return m_side;
}


void ClientSession::reset()
{
    std::lock_guard<std::mutex> lock(m_mutex);
    m_connected = false;
    clearAuthenticatedStateUnsafe();
}


void ClientSession::clearAuthenticatedStateUnsafe()
{
    m_userId.clear();
    m_username.clear();
    m_sessionId.clear();
    m_roomId.clear();
    m_side = Side::NONE;
    m_authenticated = false;
}
