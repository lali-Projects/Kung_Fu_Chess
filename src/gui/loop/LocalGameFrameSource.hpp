#pragma once

#include "IGameFrameSource.hpp"


class GameEngine;
class GameSnapshotBuilder;


/** Keeps the existing in-process engine usable behind the frame seam. */
class LocalGameFrameSource : public IGameFrameSource
{
public:
    LocalGameFrameSource(
        GameEngine& gameEngine,
        GameSnapshotBuilder& snapshotBuilder);

    std::optional<GameSnapshot> getFrame(
        int deltaTimeMs) override;

private:
    GameEngine& m_gameEngine;
    GameSnapshotBuilder& m_snapshotBuilder;
};
