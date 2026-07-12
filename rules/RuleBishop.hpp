#pragma once
#include "IMoveRule.hpp"
#include "RuleUtils.hpp"
class RuleBishop : public IMoveRule {
public:
    std::set<Position> getLegalDestinations(const Board& board, const Piece& piece) const override {
        std::set<Position> moves;
        const int diagonalDirections[4][2] = {{1, 1}, {1, -1}, {-1, 1}, {-1, -1}};
        for (const auto& dir : diagonalDirections) {
            addMovesInDirection(board, piece, dir[0], dir[1], moves);
        }
        return moves;
    }
};