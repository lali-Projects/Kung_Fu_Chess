#pragma once

#include "IGameFrameSource.hpp"


class AuthoritativeGameState;


/** Exposes only the latest server-authored snapshot to the renderer. */
class RemoteGameFrameSource : public IGameFrameSource
{
public:
    explicit RemoteGameFrameSource(
        AuthoritativeGameState& gameState);

    std::optional<GameSnapshot> getFrame(
        int deltaTimeMs) override;

private:
    AuthoritativeGameState& m_gameState;
};
