#include "Sprite.hpp"

#include <stdexcept>


void Sprite::addFrame( const Img& frame)
{
    frames.push_back(frame);
}
const Img& Sprite::getFrame( std::size_t index) const
{
    if(frames.empty())
    {
        throw std::runtime_error(
            "Cannot get frame from empty sprite"
        );
    }

    return frames[index % frames.size()];
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