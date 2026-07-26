#pragma once


#include <memory>
#include <string>
#include <vector>


#include "MoveResult.hpp"
#include "PieceTypes.hpp"



class GameController;
class GameSnapshotBuilder;
class EventBus;
class PlayerSession;
class ClickCommand;



/**
 * @brief Represents one running game inside a Room.
 *
 * Owned by Room.
 *
 * Responsibilities:
 *
 *  - Manage players.
 *  - Forward commands.
 *  - Build snapshots.
 *  - Publish game events.
 *
 *
 * Does NOT own:
 *
 *  - GameController
 *  - GameSnapshotBuilder
 *
 * They are owned by GameContext.
 */
class GameSession
{

public:


    GameSession(
        const std::string& id,
        GameController& controller,
        GameSnapshotBuilder& snapshotBuilder,
        EventBus& eventBus);



    ~GameSession();



    GameSession(
        const GameSession&) = delete;



    GameSession& operator=(
        const GameSession&) = delete;



public:


    bool addPlayer(
        std::shared_ptr<PlayerSession> player);



    MoveResult handleClick(
        PlayerSession& player,
        const ClickCommand& command);



    const std::string& getId() const;



    bool isObserver(
        const PlayerSession& player) const;



    bool containsPlayer(
        const PlayerSession& player) const;



    size_t getPlayerCount() const;



    size_t getObserverCount() const;



    const std::shared_ptr<PlayerSession>&
    getWhitePlayer() const;



    const std::shared_ptr<PlayerSession>&
    getBlackPlayer() const;



    GameSnapshotBuilder&
    getSnapshotBuilder();



    const GameSnapshotBuilder&
    getSnapshotBuilder() const;



private:


    Side assignSide();



private:


    std::string m_id;



    /*
        Not owned.

        Lifetime managed by GameContext.
    */
    GameController& m_controller;



    /*
        Not owned.

        Lifetime managed by GameContext.
    */
    GameSnapshotBuilder& m_snapshotBuilder;



    EventBus& m_eventBus;



    std::shared_ptr<PlayerSession>
        m_whitePlayer;



    std::shared_ptr<PlayerSession>
        m_blackPlayer;



    std::vector<
        std::shared_ptr<PlayerSession>>
        m_observers;

};