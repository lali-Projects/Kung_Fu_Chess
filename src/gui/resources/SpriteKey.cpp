#include "SpriteKey.hpp"



SpriteKey::SpriteKey(
    Side side,
    PieceType type,
    PieceState state
)
    :
    side(side),
    type(type),
    state(state)
{
}



Side SpriteKey::getSide() const
{
    return side;
}



PieceType SpriteKey::getType() const
{
    return type;
}



PieceState SpriteKey::getState() const
{
    return state;
}



bool SpriteKey::operator==(
    const SpriteKey& other
) const
{
    return
        side == other.side &&
        type == other.type &&
        state == other.state;
}