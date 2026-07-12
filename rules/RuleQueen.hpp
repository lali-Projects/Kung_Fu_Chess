#pragma once
#include "IMoveRule.hpp"
#include "RuleUtils.hpp"

class RuleQueen : public IMoveRule {
public:
    std::set<Position> getLegalDestinations(const Board& board, const Piece& piece) const override {
        std::set<Position> moves;
        const int allDirections[8][2] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}, {1, 1}, {1, -1}, {-1, 1}, {-1, -1}};
        for (const auto& dir : allDirections) {
            addMovesInDirection(board, piece, dir[0], dir[1], moves);
        }
        return moves;
    }
};