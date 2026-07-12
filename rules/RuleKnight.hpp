#pragma once
#include "IMoveRule.hpp"
#include "RuleUtils.hpp"

class RuleKnight : public IMoveRule {
public:
    std::set<Position> getLegalDestinations(const Board& board, const Piece& piece) const override {
        std::set<Position> moves;
        const int knightJumps[8][2] = {{2, 1}, {2, -1}, {-2, 1}, {-2, -1}, {1, 2}, {1, -2}, {-1, 2}, {-1, -2}};
        
        for (const auto& jump : knightJumps) {
            Position targetPos(piece.getPosition().getRow() + jump[0], piece.getPosition().getCol() + jump[1]);
            if (board.isInsideBoard(targetPos)) {
                auto target = board.getPieceAt(targetPos);
                if (!target || target->getSide() != piece.getSide()) moves.insert(targetPos);
            }
        }
        return moves;
    }
};