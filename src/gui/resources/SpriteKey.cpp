#pragma once

#include <string>

#include "PieceSnapshot.hpp"


class SpriteKey
{
public:

    static std::string create(const PieceSnapshot& piece)
    {
        std::string side = piece.side == Side::WHITE ? "W" : "B";


        std::string type;


        switch(piece.type)
        {
            case PieceType::KING:
                type="K";
                break;

            case PieceType::QUEEN:
                type="Q";
                break;

            case PieceType::ROOK:
                type="R";
                break;

            case PieceType::BISHOP:
                type="B";
                break;

            case PieceType::KNIGHT:
                type="N";
                break;

            case PieceType::PAWN:
                type="P";
                break;
        }


        return side + type + "_"
             + stateToString(piece.state);
    }


private:


    static std::string stateToString(
        PieceState state)
    {

        switch(state)
        {
            case PieceState::IDLE:
                return "idle";

            case PieceState::MOVING:
                return "moving";

            case PieceState::AIRBORNE:
                return "jump";
            
            default:
                return "idle";
        }
    }
};