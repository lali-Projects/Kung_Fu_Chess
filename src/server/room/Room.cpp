#include "Room.hpp"


#include "GameContext.hpp"
#include "GameSession.hpp"


#include <stdexcept>
#include <utility>





//================================================
// Constructor
//================================================

Room::Room(
    std::string id,
    std::unique_ptr<GameContext> context,
    std::unique_ptr<GameSession> session)
:
m_id(std::move(id)),
m_context(std::move(context)),
m_session(std::move(session))
{

    if(!valid())
    {
        throw std::invalid_argument(
            "Invalid Room configuration");
    }

}






//================================================
// Identity
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

    if(!m_session)
        return false;



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
        getPlayerCount()
        ==
        0;

}








//================================================
// Active
//================================================

bool Room::isActive() const
{

    if(!m_session)
        return false;



    auto state =
        m_session->getState();



    return
        state ==
        GameSession::State::RUNNING
        ||
        state ==
        GameSession::State::WAITING;

}








//================================================
// Running
//================================================

bool Room::isRunning() const
{

    if(!m_session)
        return false;



    return
        m_session->getState()
        ==
        GameSession::State::RUNNING;

}








//================================================
// Finished
//================================================

bool Room::isFinished() const
{

    if(!m_session)
        return false;



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

    if(!m_session)
        return 0;



    return
        m_session->getPlayerCount();

}








//================================================
// Validation
//================================================

bool Room::valid() const
{

    return
        !m_id.empty()
        &&
        m_context != nullptr
        &&
        m_session != nullptr;

}