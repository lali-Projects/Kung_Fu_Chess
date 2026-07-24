#pragma once


#include <memory>
#include <string>
#include <vector>

#include "GameController.hpp"
#include "GameSnapshotBuilder.hpp"
#include "MoveResult.hpp"
#include "PieceTypes.hpp"



class GameController;
class GameSnapshotBuilder;
class EventBus;
class PlayerSession;
class ClickCommand;



/**
 * @brief Represents one running Kung Fu Chess game.
 *
 * Responsibilities:
 *
 *  - Manage connected players.
 *  - Assign player roles.
 *  - Forward commands.
 *  - Publish game state changes.
 *
 *
 * Does NOT know:
 *
 *  - Movement rules.
 *  - Board logic.
 *  - Timing logic.
 *  - RealTimeArbiter internals.
 */
class GameSession
{
public:


    GameSession(
        const std::string& id,
        std::unique_ptr<GameController> controller,
        std::unique_ptr<GameSnapshotBuilder> snapshotBuilder,
        EventBus& eventBus);



public:


    bool addPlayer(
        std::shared_ptr<PlayerSession> player);



    MoveResult handleClick(
        PlayerSession& player,
        const ClickCommand& command);



    const std::string& getId() const;



    bool isObserver(
        const PlayerSession& player) const;



private:


    Side assignSide();



private:


    std::string m_id;



    /*
        Game control layer.
    */
    std::unique_ptr<GameController>
        m_controller;



    /*
        Creates immutable game snapshots.
    */
    std::unique_ptr<GameSnapshotBuilder>
        m_snapshotBuilder;



    /*
        Publishes game events.
    */
    EventBus& m_eventBus;



    std::shared_ptr<PlayerSession>
        m_whitePlayer;


    std::shared_ptr<PlayerSession>
        m_blackPlayer;



    std::vector<
        std::shared_ptr<PlayerSession>>
        m_observers;
};