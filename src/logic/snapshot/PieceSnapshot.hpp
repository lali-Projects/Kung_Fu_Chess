#pragma once

#include "PieceTypes.hpp"
#include "Position.hpp"


struct PieceSnapshot
{
    int id;

    Side side;

    PieceType type;

    Position position;

    PieceState state;
};