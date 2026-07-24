#pragma once


#include <memory>
#include <string>



class GameSession;



/**
 * @brief Manages active game sessions.
 *
 * Current implementation:
 *
 *  - Supports one active game.
 *
 * Future:
 *
 *  - Multiple games.
 *  - Match making.
 *
 *
 * Does NOT know:
 *
 *  - Network.
 *  - Players.
 *  - Commands.
 *  - Game rules.
 */
class SessionManager
{
public:


    SessionManager();


    ~SessionManager();



    SessionManager(
        const SessionManager&) = delete;


    SessionManager& operator=(
        const SessionManager&) = delete;



public:


    /**
     * @brief Adds a game session.
     *
     * Takes ownership.
     */
    void addSession(
        std::unique_ptr<GameSession> session);



    /**
     * @brief Returns current game session.
     *
     * Does not transfer ownership.
     */
    GameSession& getSession();



    /**
     * @brief Checks if session exists.
     */
    bool hasSession() const;



private:


    /*
        Current active game.

        SessionManager owns it.
    */
    std::unique_ptr<GameSession>
        m_session;
};