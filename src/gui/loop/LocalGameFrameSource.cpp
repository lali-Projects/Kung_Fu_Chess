#include "LocalGameFrameSource.hpp"

#include "GameEngine.hpp"
#include "GameSnapshotBuilder.hpp"


LocalGameFrameSource::LocalGameFrameSource(
    GameEngine& gameEngine,
    GameSnapshotBuilder& snapshotBuilder)
    : m_gameEngine(gameEngine),
      m_snapshotBuilder(snapshotBuilder)
{
}


std::optional<GameSnapshot>
LocalGameFrameSource::getFrame(int deltaTimeMs)
{
    if(deltaTimeMs > 0)
    {
        m_gameEngine.wait(deltaTimeMs);
    }
    return m_snapshotBuilder.build();
}
