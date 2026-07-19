#include "SpriteKey.hpp"


std::string SpriteKey::create(const PieceSnapshot& piece)
{
    std::string side = (piece.side == Side::WHITE) ? "W" : "B";

    return side + typeToString(piece.type) + "_" + stateToString(piece.state);
}

std::string SpriteKey::stateToString(PieceState state)
{
    switch(state)
    {
        case PieceState::IDLE:        return "idle";
        case PieceState::MOVING:      return "move";
        case PieceState::AIRBORNE:    return "jump";
        case PieceState::LONG_REST:   return "long_rest";
        case PieceState::SHORT_REST:  return "short_rest";
        default:                      return "idle";
    }
}
