#pragma once
#include "IMoveRule.hpp"
#include "RuleUtils.hpp"

class RuleKing : public IMoveRule {
public:
    std::set<Position> getLegalDestinations(const Board& board, const Piece& piece) const override {
        std::set<Position> moves;
        for (int rowDelta = -1; rowDelta <= 1; ++rowDelta) {
            for (int colDelta = -1; colDelta <= 1; ++colDelta) {
                if (rowDelta == 0 && colDelta == 0) continue;
                Position targetPos(piece.getPosition().getRow() + rowDelta, piece.getPosition().getCol() + colDelta);
                if (board.isInsideBoard(targetPos)) {
                    auto target = board.getPieceAt(targetPos);
                    if (!target || target->getSide() != piece.getSide()) moves.insert(targetPos);
                }
            }
        }
        return moves;
    }
};