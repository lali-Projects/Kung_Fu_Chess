#pragma once

#include <optional>

#include "GameSnapshot.hpp"


/** Supplies immutable frames to the presentation loop. */
class IGameFrameSource
{
public:
    virtual ~IGameFrameSource() = default;

    virtual std::optional<GameSnapshot> getFrame(
        int deltaTimeMs) = 0;
};
