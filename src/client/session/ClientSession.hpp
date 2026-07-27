#pragma once

#include <mutex>
#include <string>

#include "PieceTypes.hpp"


/** Thread-safe client-visible connection, identity and room state. */
class ClientSession
{
public:
    ClientSession() = default;

    void setConnected(bool connected);
    bool isConnected() const;

    void login(
        const std::string& userId,
        const std::string& username,
        const std::string& sessionId);
    void logout();
    bool isAuthenticated() const;

    std::string getUserId() const;
    std::string getUsername() const;
    std::string getSessionId() const;

    void setRoomId(const std::string& roomId);
    void clearRoom();
    bool hasRoom() const;
    std::string getRoomId() const;

    void setSide(Side side);
    void clearSide();
    Side getSide() const;

    /** Clears all state, including the connection flag. */
    void reset();

private:
    void clearAuthenticatedStateUnsafe();

private:
    mutable std::mutex m_mutex;
    std::string m_userId;
    std::string m_username;
    std::string m_sessionId;
    std::string m_roomId;
    Side m_side{Side::NONE};
    bool m_connected{false};
    bool m_authenticated{false};
};
