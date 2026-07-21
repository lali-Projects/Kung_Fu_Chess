#include "AnimationPlayer.hpp"
#include <algorithm>
#include "GuiConfig.hpp"

const Img& AnimationPlayer::getCurrentFrame(const Animation& animation, int animationStartTime, int currentTime) const
{
    std::size_t index = calculateFrameIndex(animation, animationStartTime, currentTime);
    return animation.getSprite().getFrame(index);
}

std::size_t AnimationPlayer::getCurrentFrameIndex(const Animation& animation, int animationStartTime, int currentTime) const
{
    return calculateFrameIndex(animation, animationStartTime, currentTime);
}

std::size_t AnimationPlayer::calculateFrameIndex(const Animation& animation, int animationStartTime, int currentTime) const
{
    const Sprite& sprite = animation.getSprite();
    if (sprite.empty())
    {
        return 0;
    }

    int elapsed = currentTime - animationStartTime;
    if (elapsed < 0)
    {
        elapsed = 0;
    }

    int fps = animation.getFramesPerSecond();
    if (fps <= 0)
    {
        return 0;
    }

    // חישוב משך הזמן של פריים בודד (במילי-שניות)
    int millisecondsPerFrame = GuiConfig::MS_PER_SECOND / fps;
    if (millisecondsPerFrame <= 0)
    {
        millisecondsPerFrame = 1;
    }

    // חישוב אינדקס הפריים
    std::size_t frame = static_cast<std::size_t>(elapsed / millisecondsPerFrame);

    if (animation.isLoop())
    {
        frame %= sprite.getFrameCount();
    }
    else
    {
        frame = std::min(frame, sprite.getFrameCount() - 1);
    }

    return frame;
}