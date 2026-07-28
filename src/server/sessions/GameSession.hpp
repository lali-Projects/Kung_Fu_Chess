#pragma once

#include <memory>
#include <mutex>
#include <optional>
#include <string>
#include <vector>

#include "MoveResult.hpp"
#include "PieceTypes.hpp"
#include "Position.hpp"

class GameContext;
class EventBus;
class PlayerSession;
class ClickCommand;
class GameSnapshot;
class PlayerInputCoordinator;

/**
 * @brief Manages the active gameplay session for a single room.
 *
 * Coordinates players, observers, input handling, game state,
 * snapshot generation, and game-state event publication.
 */
class GameSession
{
public:
    enum class State
    {
        WAITING,
        RUNNING,
        FINISHED
    };

public:
    GameSession(
        const std::string& id,
        GameContext& context,
        EventBus& eventBus
    );

    ~GameSession();

    GameSession(const GameSession&) = delete;
    GameSession& operator=(const GameSession&) = delete;

public:
    //=================================
    // Players
    //=================================
    bool addPlayer(std::shared_ptr<PlayerSession> player);

    bool removePlayer(const PlayerSession& player);

    bool containsPlayer(const PlayerSession& player) const;

    bool isObserver(const PlayerSession& player) const;

    size_t getPlayerCount() const;

    size_t getObserverCount() const;

    std::shared_ptr<PlayerSession> getWhitePlayer() const;

    std::shared_ptr<PlayerSession> getBlackPlayer() const;

    //=================================
    // Commands
    //=================================
    MoveResult handleClick(
        PlayerSession& player,
        const ClickCommand& command
    );

    std::optional<Position> getSelectedPosition(
        const PlayerSession& player
    ) const;

    void tick(
        int milliseconds,
        bool broadcastSnapshot
    );

    //=================================
    // State
    //=================================
    State getState() const;

    void setState(State state);

    bool isRunning() const;

    //=================================
    // Identity
    //=================================
    const std::string& getId() const;

private:
    Side assignSideUnsafe() const;

    bool containsPlayerUnsafe(const PlayerSession& player) const;

    size_t getPlayerCountUnsafe() const;

    GameSnapshot buildSnapshotUnsafe() const;

    void publishSnapshot(const GameSnapshot& snapshot);

private:
    std::string m_id;

    GameContext& m_context;

    EventBus& m_eventBus;

    std::shared_ptr<PlayerSession> m_whitePlayer;

    std::shared_ptr<PlayerSession> m_blackPlayer;

    std::vector<std::shared_ptr<PlayerSession>> m_observers;

    std::unique_ptr<PlayerInputCoordinator> m_inputCoordinator;

    State m_state{State::WAITING};

    mutable std::mutex m_mutex;

    mutable std::mutex m_publishMutex;
};