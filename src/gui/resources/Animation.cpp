#include "Animation.hpp"

Animation::Animation()
    :
    framesPerSecond(0),
    loop(false),
    nextState(""),
    speedMetersPerSecond(0.0)
{
}

//---------------------------------
// Sprite
//---------------------------------

void Animation::setSprite(const Sprite& newSprite)
{
    sprite = newSprite;
}

const Sprite& Animation::getSprite() const
{
    return sprite;
}

//---------------------------------
// FPS
//---------------------------------

void Animation::setFramesPerSecond(int fps)
{
    framesPerSecond = fps;
}

int Animation::getFramesPerSecond() const
{
    return framesPerSecond;
}

//---------------------------------
// Loop
//---------------------------------

void Animation::setLoop(bool value)
{
    loop = value;
}

bool Animation::isLoop() const
{
    return loop;
}

//---------------------------------
// Next state
//---------------------------------

void Animation::setNextState(const std::string& state)
{
    nextState = state;
}

const std::string& Animation::getNextState() const
{
    return nextState;
}

//---------------------------------
// Speed
//---------------------------------

void Animation::setSpeed(double speed)
{
    speedMetersPerSecond = speed;
}

double Animation::getSpeed() const
{
    return speedMetersPerSecond;
}

//---------------------------------
// Convenience
//---------------------------------

bool Animation::empty() const
{
    return sprite.empty();
}