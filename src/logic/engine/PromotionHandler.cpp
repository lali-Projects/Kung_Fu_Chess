#include "PromotionHandler.hpp"

bool PromotionHandler::handlePromotion(Piece& piece, const Board& board) 
{
    if(piece.getType() != PieceType::PAWN)
        return false;

    const int firstRow = 0;
    const int lastRow = board.getRows() - 1;
    const Position& position = piece.getPosition();

    if(piece.getSide() == Side::WHITE) 
    {
        if(position.getRow() != firstRow)
            return false;
    } 
    else 
    {
        if(position.getRow() != lastRow)
            return false;
    }

    piece.setType(PieceType::QUEEN);

    return true;
}