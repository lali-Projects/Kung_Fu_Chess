#include "PlayerSessionManager.hpp"
#include "PlayerSession.hpp"

#include <utility>

//================================================
// Create Session
//================================================

std::shared_ptr<PlayerSession> PlayerSessionManager::createSession(const std::string& sessionId)
{
    if (sessionId.empty())
    {
        return nullptr;
    }

    std::lock_guard<std::mutex> lock(m_mutex);

    if (m_sessions.find(sessionId) != m_sessions.end())
    {
        return nullptr;
    }

    auto session = std::make_shared<PlayerSession>(sessionId);
    m_sessions.emplace(sessionId, session);

    return session;
}

//================================================
// Remove Session
//================================================

bool PlayerSessionManager::removeSession(const std::string& sessionId)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    return m_sessions.erase(sessionId) > 0;
}

//================================================
// Get Session
//================================================

std::shared_ptr<PlayerSession> PlayerSessionManager::getSession(const std::string& sessionId)
{
    std::lock_guard<std::mutex> lock(m_mutex);

    auto iterator = m_sessions.find(sessionId);
    if (iterator == m_sessions.end())
    {
        return nullptr;
    }

    return iterator->second;
}

std::shared_ptr<const PlayerSession> PlayerSessionManager::getSession(const std::string& sessionId) const
{
    std::lock_guard<std::mutex> lock(m_mutex);

    auto iterator = m_sessions.find(sessionId);
    if (iterator == m_sessions.end())
    {
        return nullptr;
    }

    return iterator->second;
}

//================================================
// Find By User
//================================================

std::shared_ptr<PlayerSession> PlayerSessionManager::findByUserId(const std::string& userId)
{
    if (userId.empty())
    {
        return nullptr;
    }

    std::lock_guard<std::mutex> lock(m_mutex);

    for (auto& [id, session] : m_sessions)
    {
        if (session && session->getUserId() == userId)
        {
            return session;
        }
    }

    return nullptr;
}

//================================================
// Find By Connection
//================================================

std::shared_ptr<PlayerSession> PlayerSessionManager::findByConnectionId(int connectionId)
{
    std::lock_guard<std::mutex> lock(m_mutex);

    for (auto& [id, session] : m_sessions)
    {
        if (session && session->getConnectionId() == connectionId)
        {
            return session;
        }
    }

    return nullptr;
}

//================================================
// Exists
//================================================

bool PlayerSessionManager::exists(const std::string& sessionId) const
{
    std::lock_guard<std::mutex> lock(m_mutex);
    return m_sessions.find(sessionId) != m_sessions.end();
}

//================================================
// Size
//================================================

size_t PlayerSessionManager::size() const
{
    std::lock_guard<std::mutex> lock(m_mutex);
    return m_sessions.size();
}