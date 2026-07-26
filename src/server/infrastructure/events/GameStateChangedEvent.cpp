#include "GameStateChangedEvent.hpp"



//================================================
// Constructor
//================================================

GameStateChangedEvent::GameStateChangedEvent(
    const std::string& roomId,
    const GameSnapshot& snapshot)
:
m_roomId(roomId),
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


//================================================
// Get Room Id
//================================================

const std::string&
GameStateChangedEvent::getRoomId() const
{
    return m_roomId;
}