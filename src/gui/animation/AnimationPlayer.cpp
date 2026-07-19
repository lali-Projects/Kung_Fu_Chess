#include "AnimationPlayer.hpp"

#include <algorithm>

const Img& AnimationPlayer::getCurrentFrame(
    const Animation& animation,
    int animationStartTime,
    int currentTime) const
{
    std::size_t index =
        calculateFrameIndex(
            animation,
            animationStartTime,
            currentTime);

    return animation
        .getSprite()
        .getFrame(index);
}

std::size_t AnimationPlayer::calculateFrameIndex(
    const Animation& animation,
    int animationStartTime,
    int currentTime) const
{
    const Sprite& sprite =
        animation.getSprite();

    if (sprite.empty())
    {
        return 0;
    }

    //---------------------------------
    // elapsed time
    //---------------------------------

    int elapsed =
        currentTime - animationStartTime;

    if (elapsed < 0)
    {
        elapsed = 0;
    }

    //---------------------------------
    // fps
    //---------------------------------

    int fps =
        animation.getFramesPerSecond();

    if (fps <= 0)
    {
        return 0;
    }

    //---------------------------------
    // milliseconds per frame
    //---------------------------------

    int millisecondsPerFrame =
        1000 / fps;

    if (millisecondsPerFrame <= 0)
    {
        millisecondsPerFrame = 1;
    }

    //---------------------------------
    // frame number
    //---------------------------------

    std::size_t frame =
        static_cast<std::size_t>(
            elapsed / millisecondsPerFrame);

    //---------------------------------
    // loop
    //---------------------------------

    if (animation.isLoop())
    {
        frame %= sprite.getFrameCount();
    }
    else
    {
        frame =
            std::min(
                frame,
                sprite.getFrameCount() - 1);
    }

    return frame;
}

std::size_t AnimationPlayer::getCurrentFrameIndex(
    const Animation& animation,
    int animationStartTime,
    int currentTime) const
{
    return calculateFrameIndex(
        animation,
        animationStartTime,
        currentTime);
}