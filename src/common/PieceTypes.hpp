#pragma once

#include <string>
enum class Side
{
    WHITE,
    BLACK
};


enum class PieceType
{
    KING,
    QUEEN,
    ROOK,
    BISHOP,
    KNIGHT,
    PAWN
};


enum class PieceState
{
    IDLE,
    MOVING,
    CAPTURED,
    AIRBORNE
};

inline std::string typeToString(PieceType type) {
    switch (type) 
    {
        case PieceType::KING:   return "K";
        case PieceType::QUEEN:  return "Q";
        case PieceType::ROOK:   return "R";
        case PieceType::BISHOP: return "B";
        case PieceType::KNIGHT: return "N";
        case PieceType::PAWN:   return "P";
    }
    return "";
}
