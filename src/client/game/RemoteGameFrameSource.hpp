#pragma once

#include "IGameFrameSource.hpp"


class AuthoritativeGameState;


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
