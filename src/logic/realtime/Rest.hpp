#pragma once

#include <memory>

#include "Piece.hpp"


class Rest
{

private:

    std::shared_ptr<Piece> piece;

    PieceState state;

    PieceState nextState;

    int startTime;

    int duration;


public:


    Rest(
        std::shared_ptr<Piece> piece,
        PieceState state,
        PieceState nextState,
        int startTime,
        int duration);



    bool hasFinished(
        int currentTime) const;



    std::shared_ptr<Piece> getPiece() const;


    PieceState getState() const;


    PieceState getNextState() const;


    int getStartTime() const;


    int getDuration() const;

};