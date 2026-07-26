#include "Room.hpp"


#include <stdexcept>


#include "GameContext.hpp"
#include "GameSession.hpp"




//================================================
// Constructor
//================================================

Room::Room(
    const std::string& id,
    std::unique_ptr<GameContext> context,
    std::unique_ptr<GameSession> session)
:
m_id(id),
m_context(std::move(context)),
m_session(std::move(session))
{

    if(m_id.empty())
    {
        throw std::runtime_error(
            "Room id cannot be empty");
    }



    if(!m_context)
    {
        throw std::runtime_error(
            "Room requires GameContext");
    }



    if(!m_session)
    {
        throw std::runtime_error(
            "Room requires GameSession");
    }

}







//================================================
// Destructor
//================================================

Room::~Room() = default;







//================================================
// ID
//================================================

const std::string&
Room::getId() const
{
    return m_id;
}







//================================================
// Session
//================================================

GameSession&
Room::getSession()
{
    return *m_session;
}





const GameSession&
Room::getSession() const
{
    return *m_session;
}







//================================================
// Context
//================================================

GameContext&
Room::getContext()
{
    return *m_context;
}





const GameContext&
Room::getContext() const
{
    return *m_context;
}







//================================================
// Can Join
//================================================

bool Room::canJoin() const
{

    return
        m_session->getState()
        ==
        GameSession::State::WAITING
        &&
        m_session->getPlayerCount()
        < 
        2;

}







//================================================
// Empty
//================================================

bool Room::isEmpty() const
{

    return
        m_session->getPlayerCount()
        ==
        0;

}







//================================================
// Running
//================================================

bool Room::isRunning() const
{

    return
        m_session->isRunning();

}







//================================================
// Finished
//================================================

bool Room::isFinished() const
{

    return
        m_session->getState()
        ==
        GameSession::State::FINISHED;

}







//================================================
// Player Count
//================================================

size_t Room::getPlayerCount() const
{

    return
        m_session->getPlayerCount();

}