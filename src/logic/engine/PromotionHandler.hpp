#pragma once

#include "Board.hpp"
#include "Piece.hpp"

/**
 * @class PromotionHandler
 * @brief Handles pawn promotion.
 *
 * Responsibilities:
 * - Detect pawn promotion.
 * - Promote a pawn to a queen.
 *
 * Does NOT:
 * - Execute moves.
 * - Validate moves.
 * - Manage the board.
 * - Manage game state.
 */
class PromotionHandler
{

public:

    /**
     * @brief Promotes a pawn if it reached the last rank.
     *
     * @param piece Piece to examine.
     * @param board Current game board.
     *
     * @return true if promotion occurred.
     */
    bool handlePromotion(Piece& piece, const Board& board);

};