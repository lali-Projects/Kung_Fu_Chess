#pragma once

#include <vector>
#include "PieceSnapshot.hpp"

class GameSnapshot
{
private:

    std::vector<PieceSnapshot> pieces;

    int currentTimeMs;

    bool gameOver;

public:

    GameSnapshot();

    void addPiece(const PieceSnapshot& piece);

    const std::vector<PieceSnapshot>& getPieces() const;

    void setCurrentTime(int time);

    int getCurrentTime() const;

    void setGameOver(bool value);

    bool isGameOver() const;

    void clear();
};