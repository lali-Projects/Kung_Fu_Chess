#include "GameOverHandler.hpp"


bool GameOverHandler::isGameOver(const Piece& capturedPiece) const
{
    return capturedPiece.getType() ==  PieceType::KING;
}