#pragma once

#include "Animation.hpp"
#include "Img.hpp"

/**
 * @class AnimationPlayer
 * @brief Computes the correct frame to display for an animation at a given time.
 *
 * Responsibilities:
 * - Calculate the frame index based on temporal data.
 * - Support both looping and one-time animation sequences.
 *
 * Note: This class is limited to logical calculations and does not perform
 * actual rendering or resource management.
 */
class AnimationPlayer
{
public:
    /**
     * @brief Returns the image to be displayed.
     *
     * @param animation The animation object.
     * @param animationStartTime The time the animation started.
     * @param currentTime The current game time.
     */
    const Img& getCurrentFrame(const Animation& animation, int animationStartTime, int currentTime) const;

    /**
     * @brief Returns the current frame index.
     *
     * @param animation The animation object.
     * @param animationStartTime The time the animation started.
     * @param currentTime The current game time.
     */
    std::size_t getCurrentFrameIndex(const Animation& animation, int animationStartTime, int currentTime) const;

private:
    /**
     * @brief Internal logic to calculate the frame index.
     */
    std::size_t calculateFrameIndex(const Animation& animation, int animationStartTime, int currentTime) const;
};