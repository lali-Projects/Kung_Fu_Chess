#include "Sprite.hpp"

#include <stdexcept>


void Sprite::addFrame( const Img& frame)
{
    frames.push_back(frame);
}
const Img& Sprite::getFrame( std::size_t index) const
{
    return frames.at(index);
}

std::size_t Sprite::getFrameCount() const
{
    return frames.size();
}

bool Sprite::empty() const
{
    return frames.empty();
}

void Sprite::clear()
{
    frames.clear();
}