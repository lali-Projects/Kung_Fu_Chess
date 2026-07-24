#include "GameStateChangedEvent.hpp"



//================================================
// Constructor
//================================================

GameStateChangedEvent::GameStateChangedEvent(
    const GameSnapshot& snapshot)
    :
    m_snapshot(snapshot)
{
}



//================================================
// Get Snapshot
//================================================

const GameSnapshot&
GameStateChangedEvent::getSnapshot() const
{
    return m_snapshot;
}