#pragma once

#include <memory>
#include <mutex>
#include <string>
#include <unordered_map>

class PlayerSession;

/**
 * @brief Manages active player sessions.
 *
 * Lifetime:
 * Exists during server runtime.
 *
 * Responsibilities:
 *  - Create active player sessions.
 *  - Store connected players.
 *  - Find sessions.
 *  - Remove disconnected sessions.
 */
class PlayerSessionManager
{
public:
    PlayerSessionManager() = default;
    ~PlayerSessionManager() = default;

    PlayerSessionManager(const PlayerSessionManager&) = delete;
    PlayerSessionManager& operator=(const PlayerSessionManager&) = delete;

public:
    std::shared_ptr<PlayerSession> createSession(const std::string& sessionId);

    bool removeSession(const std::string& sessionId);

    std::shared_ptr<PlayerSession> getSession(const std::string& sessionId);

    std::shared_ptr<const PlayerSession> getSession(const std::string& sessionId) const;

    std::shared_ptr<PlayerSession> findByUserId(const std::string& userId);

    std::shared_ptr<PlayerSession> findByConnectionId(int connectionId);

    bool exists(const std::string& sessionId) const;

    size_t size() const;

private:
    mutable std::mutex m_mutex;

    std::unordered_map<
        std::string,
        std::shared_ptr<PlayerSession>
    > m_sessions;
};