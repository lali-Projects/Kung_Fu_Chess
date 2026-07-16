#pragma once

#include <vector>

#include "PieceSnapshot.hpp"


class GameSnapshot
{
private:

    std::vector<PieceSnapshot> pieces;

    int currentTime;

    bool gameOver;


public:

    GameSnapshot(
        const std::vector<PieceSnapshot>& pieces,
        int time,
        bool over)
        :
        pieces(pieces),
        currentTime(time),
        gameOver(over)
    {
    }



    const std::vector<PieceSnapshot>& getPieces() const
    {
        return pieces;
    }


    int getCurrentTime() const
    {
        return currentTime;
    }


    bool isGameOver() const
    {
        return gameOver;
    }
};