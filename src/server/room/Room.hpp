#pragma once


#include <memory>
#include <string>
#include <cstddef>



class GameContext;
class GameSession;



/**
 * @brief Represents a single game room.
 *
 * Owns:
 *
 *  - GameContext
 *  - GameSession
 *
 *
 * Responsibilities:
 *
 *  - Provide access to game session.
 *  - Provide room state information.
 *  - Control room lifetime.
 *
 *
 * Does NOT know:
 *
 *  - Network.
 *  - Authentication.
 *  - Users.
 *  - Database.
 *  - Server.
 */
class Room
{

public:


    Room(
        std::string id,
        std::unique_ptr<GameContext> context,
        std::unique_ptr<GameSession> session);



    ~Room();



    Room(
        const Room&) = delete;



    Room& operator=(
        const Room&) = delete;



    Room(
        Room&&) noexcept = default;



    Room& operator=(
        Room&&) noexcept = default;



public:


//=================================
// Identity
//=================================


    const std::string&
    getId() const;



//=================================
// Components
//=================================


    GameSession&
    getSession();



    const GameSession&
    getSession() const;



    GameContext&
    getContext();



    const GameContext&
    getContext() const;




//=================================
// State
//=================================


    bool canJoin() const;



    bool isEmpty() const;



    bool isActive() const;



    bool isRunning() const;



    bool isFinished() const;



    size_t getPlayerCount() const;



private:


    bool valid() const;



private:


    std::string m_id;



    std::unique_ptr<GameContext>
        m_context;



    std::unique_ptr<GameSession>
        m_session;

};
