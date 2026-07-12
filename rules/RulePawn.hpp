#pragma once
#include "IMoveRule.hpp"
#include "RuleUtils.hpp"

class RulePawn : public IMoveRule {
public:
   std::set<Position> getLegalDestinations(const Board& board, const Piece& piece) const override {
    std::set<Position> moves;
    int r = piece.getPosition().getRow();
    int c = piece.getPosition().getCol();
    int forwardStep = (piece.getSide() == Side::WHITE) ? -1 : 1;
    
    // 1. צעד יחיד
    Position forwardPos(r + forwardStep, c);
    if (board.isInsideBoard(forwardPos) && !board.getPieceAt(forwardPos)) {
        moves.insert(forwardPos);

        // 2. צעד כפול: בדיקה שהצעד הראשון פנוי וגם השני פנוי
        // שורת התחלה: לבן בשורה 3 (לפי הדוגמה שלך), שחור בשורה 0
        int startRow = (piece.getSide() == Side::WHITE) ? 3 : 0; 
        Position doubleStepPos(r + (2 * forwardStep), c);
        
        if (r == startRow && board.isInsideBoard(doubleStepPos) && !board.getPieceAt(doubleStepPos)) {
            moves.insert(doubleStepPos);
        }
    }

        // 3. אכילה באלכסון
        for (int sideDelta : {-1, 1}) {
            Position attackPos(r + forwardStep, c + sideDelta);
            if (board.isInsideBoard(attackPos)) {
                auto target = board.getPieceAt(attackPos);
                // אכילה חוקית רק אם יש כלי של היריב
                if (target && target->getSide() != piece.getSide()) {
                    moves.insert(attackPos);
                }
            }
        }
        
        return moves;
    }
};