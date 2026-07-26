#include "GameSession.hpp"


#include <iostream>


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
    std::cout
        << "[SESSION] Click received"
        << std::endl;


    //---------------------------------
    // Observer check
    //---------------------------------

    if(isObserver(player))
    {
        std::cout
            << "[SESSION] Observer rejected"
            << std::endl;

        return
        {
            false,
            "observer_cannot_play"
        };
    }



    //---------------------------------
    // Controller check
    //---------------------------------

    if(!m_controller)
    {
        std::cout
            << "[SESSION] Controller missing"
            << std::endl;

        return
        {
            false,
            "controller_missing"
        };
    }



    //---------------------------------
    // Forward click to controller
    //---------------------------------

    std::cout
        << "[SESSION] Forwarding click to controller"
        << std::endl;


    MoveResult result =
        m_controller->click(
            command.getPosition());



    std::cout
        << "[SESSION RESULT] "
        << result.success
        << " "
        << result.reason
        << std::endl;



    //---------------------------------
    // Only successful actions update state
    //---------------------------------

    if(result.success)
    {

        std::cout
            << "[SESSION] ENTER SUCCESS BLOCK"
            << std::endl;



        //---------------------------------
        // Snapshot builder check
        //---------------------------------

        if(!m_snapshotBuilder)
        {
            std::cout
                << "[SESSION] SNAPSHOT BUILDER MISSING"
                << std::endl;


            return result;
        }



        //---------------------------------
        // Build snapshot
        //---------------------------------

        std::cout
            << "[SESSION] BEFORE BUILD"
            << std::endl;



        GameSnapshot snapshot =
            m_snapshotBuilder->build();



        std::cout
            << "[SESSION] AFTER BUILD"
            << std::endl;



        //---------------------------------
        // Publish event
        //---------------------------------

        std::cout
            << "[SESSION] BEFORE PUBLISH"
            << std::endl;



        m_eventBus.publish(
            std::make_shared<GameStateChangedEvent>(
                snapshot));



        std::cout
            << "[SESSION] AFTER PUBLISH"
            << std::endl;

    }
    else
    {
        std::cout
            << "[SESSION] Move failed - no snapshot"
            << std::endl;
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

//================================================
// Get White Player
//================================================

const std::shared_ptr<PlayerSession>&
GameSession::getWhitePlayer() const
{
    return m_whitePlayer;
}



//================================================
// Get Black Player
//================================================

const std::shared_ptr<PlayerSession>&
GameSession::getBlackPlayer() const
{
    return m_blackPlayer;
}



//================================================
// Get Observer Count
//================================================

size_t GameSession::getObserverCount() const
{
    return m_observers.size();
}