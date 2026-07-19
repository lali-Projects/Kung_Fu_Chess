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

    int animationStartTime = 0;

    bool hasActiveAnimation = false;

     bool hasMotion = false;

    Position motionStart;


    Position motionDestination;


    int motionStartTime = 0;


    int motionEndTime = 0;
};