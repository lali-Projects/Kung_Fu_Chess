#pragma once
#include "IMoveRule.hpp"
#include "RuleUtils.hpp"

class RulePawn : public IMoveRule {
public:
  std::set<Position> getLegalDestinations(const Board& board, const Piece& piece) const override {
    std::set<Position> moves;
    int r = piece.getPosition().getRow();
    int c = piece.getPosition().getCol();
    int rows = board.getRows();
    
    // כיוון התנועה
    int forwardStep = (piece.getSide() == Side::WHITE) ? -1 : 1;
    
    // קביעת שורת התחלה דינמית לפי גודל הלוח:
    // לבן מתחיל בשורה הלפני אחרונה (rows - 2)
    // שחור מתחיל בשורה השנייה (1)
    int startRow = (piece.getSide() == Side::WHITE) ? rows - 2 : 1;
    
    // 1. צעד יחיד
    Position forwardPos(r + forwardStep, c);
    if (board.isInsideBoard(forwardPos) && !board.getPieceAt(forwardPos)) {
        moves.insert(forwardPos);

        // 2. צעד כפול
        Position doubleStepPos(r + (2 * forwardStep), c);
        
        // בדיקה: האם אנחנו בשורת ההתחלה?
        if (r == startRow && board.isInsideBoard(doubleStepPos) && !board.getPieceAt(doubleStepPos)) {
            moves.insert(doubleStepPos);
        }
    }
    
    // 3. אכילה באלכסון (להשאיר כפי שהיה)
    for (int sideDelta : {-1, 1}) {
        Position attackPos(r + forwardStep, c + sideDelta);
        if (board.isInsideBoard(attackPos)) {
            auto target = board.getPieceAt(attackPos);
            if (target && target->getSide() != piece.getSide()) {
                moves.insert(attackPos);
            }
        }
    }
    return moves;
}
};