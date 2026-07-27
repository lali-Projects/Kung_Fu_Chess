#include "ClientSession.hpp"


//================================================
// Login
//================================================

void ClientSession::login(
    const std::string& userId,
    const std::string& username,
    const std::string& sessionId)
{
    m_userId = userId;

    m_username = username;

    m_sessionId = sessionId;


    m_authenticated = true;
}



//================================================
// Logout
//================================================

void ClientSession::logout()
{
    reset();
}



//================================================
// Auth State
//================================================

bool ClientSession::isAuthenticated() const
{
    return m_authenticated;
}



//================================================
// Identity
//================================================

const std::string&
ClientSession::getUserId() const
{
    return m_userId;
}



const std::string&
ClientSession::getUsername() const
{
    return m_username;
}



const std::string&
ClientSession::getSessionId() const
{
    return m_sessionId;
}



//================================================
// Room
//================================================

void ClientSession::setRoomId(
    const std::string& roomId)
{
    m_roomId = roomId;
}



void ClientSession::clearRoom()
{
    m_roomId.clear();
}



bool ClientSession::hasRoom() const
{
    return !m_roomId.empty();
}



const std::string&
ClientSession::getRoomId() const
{
    return m_roomId;
}



//================================================
// Reset
//================================================

void ClientSession::reset()
{
    m_userId.clear();

    m_username.clear();

    m_sessionId.clear();

    m_roomId.clear();


    m_authenticated = false;
}