#include "GameSession.hpp"


#include "GameContext.hpp"

#include "GameController.hpp"
#include "GameSnapshotBuilder.hpp"

#include "GameSnapshot.hpp"
#include "GameStateChangedEvent.hpp"

#include "EventBus.hpp"

#include "PlayerSession.hpp"
#include "ClickCommand.hpp"


#include <algorithm>





//=================================
// Constructor
//=================================

GameSession::GameSession(
    const std::string& id,
    GameContext& context,
    EventBus& eventBus)
:
m_id(id),
m_context(context),
m_eventBus(eventBus)
{
}





GameSession::~GameSession() = default;







//=================================
// Add Player
//=================================

bool GameSession::addPlayer(
    std::shared_ptr<PlayerSession> player)
{

    if(!player)
        return false;


    if(player->hasRoom())
        return false;



    Side side =
        assignSide();



    player->setSide(side);

    player->setRoomId(
        m_id);



    switch(side)
    {

        case Side::WHITE:
            m_whitePlayer = player;
            break;



        case Side::BLACK:
            m_blackPlayer = player;
            break;



        case Side::OBSERVER:
            m_observers.push_back(player);
            break;

    }



    if(m_whitePlayer &&
       m_blackPlayer)
    {
        m_state =
            State::RUNNING;
    }



    return true;
}







//=================================
// Remove Player
//=================================

bool GameSession::removePlayer(
    const PlayerSession& player)
{

    if(m_whitePlayer &&
       m_whitePlayer.get() == &player)
    {
        m_whitePlayer.reset();
        return true;
    }



    if(m_blackPlayer &&
       m_blackPlayer.get() == &player)
    {
        m_blackPlayer.reset();
        return true;
    }



    auto oldSize =
        m_observers.size();



    m_observers.erase(
        std::remove_if(
            m_observers.begin(),
            m_observers.end(),
            [&](const auto& observer)
            {
                return observer &&
                       observer.get() == &player;
            }),
        m_observers.end());



    return
        oldSize != m_observers.size();
}







//=================================
// Assign Side
//=================================

Side GameSession::assignSide()
{

    if(!m_whitePlayer)
        return Side::WHITE;



    if(!m_blackPlayer)
        return Side::BLACK;



    return Side::OBSERVER;
}







//=================================
// Handle Click
//=================================

MoveResult GameSession::handleClick(
    PlayerSession& player,
    const ClickCommand& command)
{

    if(m_state != State::RUNNING)
    {
        return
        {
            false,
            "game_not_running"
        };
    }



    if(!player.isConnected())
    {
        return
        {
            false,
            "player_disconnected"
        };
    }



    if(!containsPlayer(player))
    {
        return
        {
            false,
            "player_not_in_session"
        };
    }



    if(isObserver(player))
    {
        return
        {
            false,
            "observer_cannot_play"
        };
    }





    MoveResult result =
        m_context.getController()
            .click(
                command.getPosition());



    if(result.success)
    {
        publishSnapshot();
    }



    return result;
}







//=================================
// Snapshot
//=================================

void GameSession::publishSnapshot()
{

    GameSnapshot snapshot =
        m_context.getSnapshotBuilder()
            .build();



    m_eventBus.publish(
        std::make_shared<GameStateChangedEvent>(
            m_id,
            snapshot));
}







//=================================
// Contains
//=================================

bool GameSession::containsPlayer(
    const PlayerSession& player) const
{

    if(m_whitePlayer &&
       m_whitePlayer.get() == &player)
        return true;



    if(m_blackPlayer &&
       m_blackPlayer.get() == &player)
        return true;



    for(const auto& observer :
        m_observers)
    {
        if(observer &&
           observer.get() == &player)
            return true;
    }



    return false;
}







bool GameSession::isObserver(
    const PlayerSession& player) const
{
    return
        player.getSide()
        ==
        Side::OBSERVER;
}







//=================================
// Getters
//=================================

const std::string&
GameSession::getId() const
{
    return m_id;
}



size_t GameSession::getPlayerCount() const
{

    size_t count = 0;


    if(m_whitePlayer)
        count++;


    if(m_blackPlayer)
        count++;


    count +=
        m_observers.size();



    return count;
}



size_t GameSession::getObserverCount() const
{
    return m_observers.size();
}




const std::shared_ptr<PlayerSession>&
GameSession::getWhitePlayer() const
{
    return m_whitePlayer;
}



const std::shared_ptr<PlayerSession>&
GameSession::getBlackPlayer() const
{
    return m_blackPlayer;
}





GameSession::State
GameSession::getState() const
{
    return m_state;
}



void GameSession::setState(
    State state)
{
    m_state = state;
}



bool GameSession::isRunning() const
{
    return
        m_state == State::RUNNING;
}