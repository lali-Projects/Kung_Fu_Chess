#include "AuthoritativeGameState.hpp"

#include <utility>


void AuthoritativeGameState::update(GameSnapshot snapshot)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    m_snapshot = std::move(snapshot);
}


std::optional<GameSnapshot>
AuthoritativeGameState::getSnapshot() const
{
    std::lock_guard<std::mutex> lock(m_mutex);
    if(!m_snapshot)
    {
        return std::nullopt;
    }

    GameSnapshot snapshot = *m_snapshot;
    if(m_hasSelectionResponse)
    {
        snapshot.setSelectedPosition(m_selectedPosition);
    }
    return snapshot;
}


bool AuthoritativeGameState::hasSnapshot() const
{
    std::lock_guard<std::mutex> lock(m_mutex);
    return m_snapshot.has_value();
}


void AuthoritativeGameState::setSelectedPosition(
    const std::optional<Position>& position)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    m_selectedPosition = position;
    m_hasSelectionResponse = true;
}


void AuthoritativeGameState::clear()
{
    std::lock_guard<std::mutex> lock(m_mutex);
    m_snapshot.reset();
    m_selectedPosition.reset();
    m_hasSelectionResponse = false;
}
