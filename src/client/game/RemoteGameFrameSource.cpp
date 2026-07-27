#include "RemoteGameFrameSource.hpp"

#include "AuthoritativeGameState.hpp"


RemoteGameFrameSource::RemoteGameFrameSource(
    AuthoritativeGameState& gameState)
    : m_gameState(gameState)
{
}


std::optional<GameSnapshot>
RemoteGameFrameSource::getFrame(int deltaTimeMs)
{
    (void)deltaTimeMs;
    return m_gameState.getSnapshot();
}
