#include "GameSession.hpp"

#include <utility>


//================================================
// Constructor
//================================================

GameSession::GameSession(
    std::string id,
    std::unique_ptr<GameController> controller,
    EventBus& eventBus)
    :
    m_id(std::move(id)),
    m_eventBus(eventBus),
    m_controller(std::move(controller))
{
}



//================================================
// Get ID
//================================================

const std::string& GameSession::getId() const
{
    return m_id;
}



//================================================
// Add Player
//================================================

bool GameSession::addPlayer(
    std::shared_ptr<PlayerSession> player)
{
    if (!player)
    {
        return false;
    }


    /*
        A chess game contains only two players.
    */
    if (m_players.size() >= 2)
    {
        return false;
    }



    /*
        Assign player side.

        First player:
            WHITE

        Second player:
            BLACK
    */
    if (m_players.empty())
    {
        player->setSide(Side::WHITE);
    }
    else
    {
        player->setSide(Side::BLACK);
    }



    m_players.push_back(
        std::move(player));


    return true;
}



//================================================
// Handle Click
//================================================

MoveResult GameSession::handleClick(
    const Position& position)
{
    if (!m_controller)
    {
        return {
            false,
            "controller_not_initialized"
        };
    }


    /*
        GameSession does not know:
            - how a move works
            - how jumping works
            - how rules are checked

        It only forwards the request.

        The flow continues:

        GameController
              |
              v
        GameEngine
    */

    return m_controller->click(position);
}