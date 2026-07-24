#include "LocalClientTransport.hpp"



//================================================
// Send Snapshot
//================================================

void LocalClientTransport::sendSnapshot(
    const GameSnapshot& snapshot)
{
    m_lastSnapshot = snapshot;
}



//================================================
// Last Snapshot
//================================================

const std::optional<GameSnapshot>&
LocalClientTransport::getLastSnapshot() const
{
    return m_lastSnapshot;
}