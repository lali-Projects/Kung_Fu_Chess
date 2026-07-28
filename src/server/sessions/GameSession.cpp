#include "GameSession.hpp"

#include <algorithm>
#include <optional>
#include <utility>

#include "ClickCommand.hpp"
#include "EventBus.hpp"
#include "GameContext.hpp"
#include "GameEngine.hpp"
#include "GameSnapshot.hpp"
#include "GameSnapshotBuilder.hpp"
#include "GameStateChangedEvent.hpp"
#include "PlayerInputCoordinator.hpp"
#include "PlayerSession.hpp"

GameSession::GameSession(
    const std::string& id,
    GameContext& context,
    EventBus& eventBus
)
    : m_id(id)
    , m_context(context)
    , m_eventBus(eventBus)
    , m_inputCoordinator(std::make_unique<PlayerInputCoordinator>(context))
{
}

GameSession::~GameSession() = default;

bool GameSession::addPlayer(std::shared_ptr<PlayerSession> player)
{
    if (!player)
    {
        return false;
    }

    std::optional<GameSnapshot> snapshot;
    std::unique_lock<std::mutex> publication;

    {
        std::lock_guard<std::mutex> lock(m_mutex);

        if ((m_state != State::WAITING && m_state != State::RUNNING) ||
            !player->isAuthenticated() ||
            !player->isConnected() ||
            player->hasRoom())
        {
            return false;
        }

        const Side side = assignSideUnsafe();

        if (side == Side::NONE)
        {
            return false;
        }

        player->setSide(side);
        player->setRoomId(m_id);

        if (side == Side::WHITE)
        {
            m_whitePlayer = player;
        }
        else if (side == Side::BLACK)
        {
            m_blackPlayer = player;
        }
        else
        {
            m_observers.push_back(player);
        }

        if (side != Side::OBSERVER)
        {
            m_inputCoordinator->addPlayer(*player);
        }

        if (m_whitePlayer && m_blackPlayer)
        {
            m_state = State::RUNNING;
        }

        snapshot = buildSnapshotUnsafe();
        publication = std::unique_lock<std::mutex>(m_publishMutex);
    }

    publishSnapshot(*snapshot);
    return true;
}

bool GameSession::removePlayer(const PlayerSession& player)
{
    std::shared_ptr<PlayerSession> removedPlayer;
    bool removedActivePlayer = false;
    std::optional<GameSnapshot> snapshot;
    std::unique_lock<std::mutex> publication;

    {
        std::lock_guard<std::mutex> lock(m_mutex);

        if (m_whitePlayer && m_whitePlayer.get() == &player)
        {
            removedPlayer = m_whitePlayer;
            m_whitePlayer.reset();
            removedActivePlayer = true;
        }
        else if (m_blackPlayer && m_blackPlayer.get() == &player)
        {
            removedPlayer = m_blackPlayer;
            m_blackPlayer.reset();
            removedActivePlayer = true;
        }
        else
        {
            auto iterator = std::find_if(
                m_observers.begin(),
                m_observers.end(),
                [&player](const auto& observer)
                {
                    return observer && observer.get() == &player;
                }
            );

            if (iterator != m_observers.end())
            {
                removedPlayer = *iterator;
                m_observers.erase(iterator);
            }
        }

        if (!removedPlayer)
        {
            return false;
        }

        m_inputCoordinator->removePlayer(*removedPlayer);

        removedPlayer->leaveGame();

        if (removedActivePlayer && m_state != State::FINISHED)
        {
            m_state = State::WAITING;
        }

        snapshot = buildSnapshotUnsafe();
        publication = std::unique_lock<std::mutex>(m_publishMutex);
    }

    publishSnapshot(*snapshot);
    return true;
}

MoveResult GameSession::handleClick(
    PlayerSession& player,
    const ClickCommand& command
)
{
    MoveResult result;
    std::optional<GameSnapshot> snapshot;
    std::unique_lock<std::mutex> publication;

    {
        std::lock_guard<std::mutex> lock(m_mutex);

        if (!player.isConnected())
        {
            return {false, "player_disconnected"};
        }

        if (!player.isAuthenticated())
        {
            return {false, "authentication_required"};
        }

        if (!containsPlayerUnsafe(player))
        {
            return {false, "player_not_in_session"};
        }

        if (player.getSide() == Side::OBSERVER || player.getSide() == Side::NONE)
        {
            return {false, "observer_cannot_play"};
        }

        if (m_state != State::RUNNING)
        {
            return {false, "game_not_running"};
        }

        result = m_inputCoordinator->handleClick(player, command);

        if (result.success)
        {
            snapshot = buildSnapshotUnsafe();
            publication = std::unique_lock<std::mutex>(m_publishMutex);
        }
    }

    if (snapshot)
    {
        publishSnapshot(*snapshot);
    }

    return result;
}

std::optional<Position> GameSession::getSelectedPosition(
    const PlayerSession& player
) const
{
    std::lock_guard<std::mutex> lock(m_mutex);

    return m_inputCoordinator->getSelectedPosition(player);
}

void GameSession::tick(
    int milliseconds,
    bool broadcastSnapshot
)
{
    if (milliseconds <= 0)
    {
        return;
    }

    std::optional<GameSnapshot> snapshot;
    std::unique_lock<std::mutex> publication;

    {
        std::lock_guard<std::mutex> lock(m_mutex);

        if (m_state != State::RUNNING)
        {
            return;
        }

        m_context.getEngine().wait(milliseconds);

        const bool gameFinished = m_context.getEngine().gameOver();

        if (gameFinished)
        {
            m_state = State::FINISHED;
        }

        if (broadcastSnapshot || gameFinished)
        {
            snapshot = buildSnapshotUnsafe();
            publication = std::unique_lock<std::mutex>(m_publishMutex);
        }
    }

    if (snapshot)
    {
        publishSnapshot(*snapshot);
    }
}

bool GameSession::containsPlayer(const PlayerSession& player) const
{
    std::lock_guard<std::mutex> lock(m_mutex);
    return containsPlayerUnsafe(player);
}

bool GameSession::containsPlayerUnsafe(const PlayerSession& player) const
{
    if (m_whitePlayer && m_whitePlayer.get() == &player)
    {
        return true;
    }

    if (m_blackPlayer && m_blackPlayer.get() == &player)
    {
        return true;
    }

    return std::any_of(
        m_observers.begin(),
        m_observers.end(),
        [&player](const auto& observer)
        {
            return observer && observer.get() == &player;
        }
    );
}

bool GameSession::isObserver(const PlayerSession& player) const
{
    std::lock_guard<std::mutex> lock(m_mutex);
    return player.getSide() == Side::OBSERVER;
}

Side GameSession::assignSideUnsafe() const
{
    if (!m_whitePlayer)
    {
        return Side::WHITE;
    }

    if (!m_blackPlayer)
    {
        return Side::BLACK;
    }

    return Side::OBSERVER;
}

size_t GameSession::getPlayerCount() const
{
    std::lock_guard<std::mutex> lock(m_mutex);
    return getPlayerCountUnsafe();
}

size_t GameSession::getPlayerCountUnsafe() const
{
    size_t count = m_observers.size();

    if (m_whitePlayer)
    {
        ++count;
    }

    if (m_blackPlayer)
    {
        ++count;
    }

    return count;
}

size_t GameSession::getObserverCount() const
{
    std::lock_guard<std::mutex> lock(m_mutex);
    return m_observers.size();
}

std::shared_ptr<PlayerSession> GameSession::getWhitePlayer() const
{
    std::lock_guard<std::mutex> lock(m_mutex);
    return m_whitePlayer;
}

std::shared_ptr<PlayerSession> GameSession::getBlackPlayer() const
{
    std::lock_guard<std::mutex> lock(m_mutex);
    return m_blackPlayer;
}

GameSession::State GameSession::getState() const
{
    std::lock_guard<std::mutex> lock(m_mutex);
    return m_state;
}

void GameSession::setState(State state)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    m_state = state;
}

bool GameSession::isRunning() const
{
    std::lock_guard<std::mutex> lock(m_mutex);
    return m_state == State::RUNNING;
}

const std::string& GameSession::getId() const
{
    return m_id;
}

GameSnapshot GameSession::buildSnapshotUnsafe() const
{
    return m_context.getSnapshotBuilder().build();
}

void GameSession::publishSnapshot(const GameSnapshot& snapshot)
{
    m_eventBus.publish(
        std::make_shared<GameStateChangedEvent>(m_id, snapshot)
    );
}