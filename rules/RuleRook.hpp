#pragma once
#include "IMoveRule.hpp"
#include "RuleUtils.hpp"

class RuleRook : public IMoveRule {
public:
    std::set<Position> getLegalDestinations(const Board& board, const Piece& piece) const override {
        std::set<Position> moves;
        const int orthogonalDirections[4][2] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};
        for (const auto& dir : orthogonalDirections) {
            addMovesInDirection(board, piece, dir[0], dir[1], moves);
        }
        return moves;
    }
};