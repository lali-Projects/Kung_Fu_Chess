#pragma once
#include "IMoveRule.hpp"


/**
 * @brief סורק את הלוח בכיוון (rowStep, colStep) עד למכשול.
 * @param board הלוח הנוכחי.
 * @param piece הכלי המבצע את המהלך.
 * @param rowStep השינוי בשורה בכל צעד.
 * @param colStep השינוי בעמודה בכל צעד.
 * @param legalMoves הקבוצה אליה יוכנסו המהלכים החוקיים.
 */
inline void addMovesInDirection(const Board& board, const Piece& piece, 
                                int rowStep, int colStep, 
                                std::set<Position>& legalMoves) {
    Position currentPos = piece.getPosition();
    int nextRow = currentPos.getRow() + rowStep;
    int nextCol = currentPos.getCol() + colStep;

    while (board.isInsideBoard(Position(nextRow, nextCol))) {
        Position targetPos(nextRow, nextCol);
        auto targetPiece = board.getPieceAt(targetPos);

        if (!targetPiece) {
            legalMoves.insert(targetPos);
        } else {
            if (targetPiece->getSide() != piece.getSide()) {
                legalMoves.insert(targetPos);
            }
            break; // עצירה בגלל חסימה
        }
        nextRow += rowStep;
        nextCol += colStep;
    }
}