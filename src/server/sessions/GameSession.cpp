#include "GameSession.hpp"

#include "GameController.hpp"
#include "GameSnapshotBuilder.hpp"
#include "GameStateChangedEvent.hpp"

#include "PlayerSession.hpp"
#include "ClickCommand.hpp"
#include "EventBus.hpp"



//================================================
// Constructor
//================================================

GameSession::GameSession(
    const std::string& id,
    std::unique_ptr<GameController> controller,
    std::unique_ptr<GameSnapshotBuilder> snapshotBuilder,
    EventBus& eventBus)
    :
    m_id(id),
    m_controller(std::move(controller)),
    m_snapshotBuilder(std::move(snapshotBuilder)),
    m_eventBus(eventBus)
{
}



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



    Side side =
        assignSide();



    player->setSide(
        side);



    switch(side)
    {

        case Side::WHITE:

            m_whitePlayer = player;

            break;



        case Side::BLACK:

            m_blackPlayer = player;

            break;



        case Side::OBSERVER:

            m_observers.push_back(
                player);

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

    /*
        Observers cannot play.
    */

    if(isObserver(player))
    {
        return
        {
            false,
            "observer_cannot_play"
        };
    }



    /*
        No turns exist in Kung Fu Chess.

        Both players can send commands.
    */

    MoveResult result =
        m_controller->click(
            command.getPosition());



    /*
        Only successful changes
        create a new snapshot.
    */

    if(result.success)
    {

        GameSnapshot snapshot =
            m_snapshotBuilder->build();



        m_eventBus.publish(
    std::make_shared<GameStateChangedEvent>(
        snapshot));
    }



    return result;
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
// Get Id
//================================================

const std::string&
GameSession::getId() const
{
    return m_id;
}