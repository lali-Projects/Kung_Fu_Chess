#pragma once


#include <memory>
#include <string>
#include <cstddef>


class GameContext;
class GameSession;



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


    Room& operator=(const Room&) = delete;



public:


    const std::string&
    getId() const;



    GameSession&
    getSession();



    const GameSession&
    getSession() const;



    GameContext&
    getContext();



    const GameContext&
    getContext() const;



public:


    bool canJoin() const;


    bool isEmpty() const;


    bool isRunning() const;


    bool isFinished() const;


    size_t getPlayerCount() const;



private:


    std::string m_id;



    std::unique_ptr<GameContext>
        m_context;



    std::unique_ptr<GameSession>
        m_session;

};