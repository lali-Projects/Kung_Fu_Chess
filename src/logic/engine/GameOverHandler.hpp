#pragma once

#include "Piece.hpp"


class GameOverHandler
{

public:

    bool isGameOver(
        const Piece& capturedPiece) const;

};