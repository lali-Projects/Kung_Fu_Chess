#pragma once


#include <memory>
#include <string>



class GameContext;
class GameSession;



/**
 * @brief Represents one active game room.
 *
 * Owns:
 *
 *  - GameContext
 *  - GameSession
 *
 *
 * Responsibilities:
 *
 *  - Maintain one running game instance.
 *  - Provide access to game services.
 *
 *
 * Does NOT know:
 *
 *  - Server
 *  - Network
 *  - Database
 */
class Room
{

public:


    Room(
        const std::string& id,
        std::unique_ptr<GameContext> context,
        std::unique_ptr<GameSession> session);



    ~Room();



    Room(
        const Room&) = delete;



    Room& operator=(
        const Room&) = delete;



public:


    const std::string& getId() const;



    GameSession& getSession();



    const GameSession& getSession() const;



    GameContext& getContext();



    const GameContext& getContext() const;



    bool isEmpty() const;



private:


    std::string m_id;



    std::unique_ptr<GameContext>
        m_context;



    std::unique_ptr<GameSession>
        m_session;

};