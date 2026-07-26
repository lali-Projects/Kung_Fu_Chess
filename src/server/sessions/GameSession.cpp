#include "GameSession.hpp"


#include "GameController.hpp"
#include "GameSnapshotBuilder.hpp"
#include "GameStateChangedEvent.hpp"

#include "PlayerSession.hpp"
#include "ClickCommand.hpp"

#include "EventBus.hpp"

#include <stdexcept>





//================================================
// Constructor
//================================================

GameSession::GameSession(
    const std::string& id,
    GameController& controller,
    GameSnapshotBuilder& snapshotBuilder,
    EventBus& eventBus)
:
m_id(id),
m_controller(controller),
m_snapshotBuilder(snapshotBuilder),
m_eventBus(eventBus)
{
}






//================================================
// Destructor
//================================================

GameSession::~GameSession() = default;







//================================================
// Add Player
//================================================

bool GameSession::addPlayer(
    std::shared_ptr<PlayerSession> player)
{

    if(!player)
    {
        return false;
    }



    if(player->hasRoom())
    {
        return false;
    }



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



    return true;
}







//================================================
// Assign Side
//================================================

Side GameSession::assignSide()
{

    if(!m_whitePlayer)
    {
        return Side::WHITE;
    }



    if(!m_blackPlayer)
    {
        return Side::BLACK;
    }



    return Side::OBSERVER;
}








//================================================
// Handle Click
//================================================

MoveResult GameSession::handleClick(
    PlayerSession& player,
    const ClickCommand& command)
{

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
        m_controller.click(
            command.getPosition());





    if(result.success)
    {

        GameSnapshot snapshot =
            m_snapshotBuilder.build();



        m_eventBus.publish(
            std::make_shared<GameStateChangedEvent>(
                m_id,
                snapshot));
    }



    return result;
}







//================================================
// Contains Player
//================================================

bool GameSession::containsPlayer(
    const PlayerSession& player) const
{

    if(m_whitePlayer &&
       m_whitePlayer.get() == &player)
    {
        return true;
    }



    if(m_blackPlayer &&
       m_blackPlayer.get() == &player)
    {
        return true;
    }



    for(const auto& observer :
        m_observers)
    {

        if(observer &&
           observer.get() == &player)
        {
            return true;
        }

    }



    return false;
}







//================================================
// Is Observer
//================================================

bool GameSession::isObserver(
    const PlayerSession& player) const
{

    return
        player.getSide()
        ==
        Side::OBSERVER;

}







//================================================
// ID
//================================================

const std::string&
GameSession::getId() const
{
    return m_id;
}







//================================================
// Count
//================================================

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







//================================================
// Observer Count
//================================================

size_t GameSession::getObserverCount() const
{
    return m_observers.size();
}







//================================================
// White
//================================================

const std::shared_ptr<PlayerSession>&
GameSession::getWhitePlayer() const
{
    return m_whitePlayer;
}







//================================================
// Black
//================================================

const std::shared_ptr<PlayerSession>&
GameSession::getBlackPlayer() const
{
    return m_blackPlayer;
}







//================================================
// Snapshot Builder
//================================================

GameSnapshotBuilder&
GameSession::getSnapshotBuilder()
{
    return m_snapshotBuilder;
}







const GameSnapshotBuilder&
GameSession::getSnapshotBuilder() const
{
    return m_snapshotBuilder;
}