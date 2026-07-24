#include "SessionManager.hpp"


#include "GameSession.hpp"



//================================================
// Constructor
//================================================

SessionManager::SessionManager()
{
}



//================================================
// Destructor
//================================================

SessionManager::~SessionManager()
{
}



//================================================
// Add Session
//================================================

void SessionManager::addSession(
    std::unique_ptr<GameSession> session)
{

    m_session =
        std::move(session);
}



//================================================
// Get Session
//================================================

GameSession&
SessionManager::getSession()
{

    return *m_session;
}



//================================================
// Has Session
//================================================

bool SessionManager::hasSession() const
{
    return m_session != nullptr;
}