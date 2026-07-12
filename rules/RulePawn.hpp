#pragma once
#include "IMoveRule.hpp"
#include "RuleUtils.hpp"
class RulePawn : public IMoveRule {
public:
    std::set<Position> getLegalDestinations(const Board& board, const Piece& piece) const override {
        std::set<Position> moves;
        int forwardStep = (piece.getSide() == Side::WHITE) ? -1 : 1;
        int r = piece.getPosition().getRow();
        int c = piece.getPosition().getCol();

        // צעד יחיד קדימה
        Position forwardPos(r + forwardStep, c);
        if (board.isInsideBoard(forwardPos) && !board.getPieceAt(forwardPos)) moves.insert(forwardPos);
        
        // אכילה באלכסון
        for (int sideDelta : {-1, 1}) {
            Position attackPos(r + forwardStep, c + sideDelta);
            if (board.isInsideBoard(attackPos)) {
                auto target = board.getPieceAt(attackPos);
                if (target && target->getSide() != piece.getSide()) moves.insert(attackPos);
            }
        }
        return moves;
    }
};