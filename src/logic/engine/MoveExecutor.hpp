#pragma once

#include <memory>

#include "Board.hpp"
#include "Piece.hpp"
#include "Position.hpp"


/**
 * @struct MoveExecutionResult
 * @brief Information about the result of the actual move execution.
 *
 * MoveExecutor does not decide the meaning of the result.
 * It only reports what happened.
 */
struct MoveExecutionResult
{
    std::shared_ptr<Piece> movingPiece;
    std::shared_ptr<Piece> capturedPiece;
    bool wasCapture = false;

};

/**
 * @class MoveExecutor
 *
 * @brief Executes an approved move on the board.
 *
 * Responsible for:
 * - Moving a piece
 * - Removing a piece located at the destination
 * - Returning information about the action
 *
 * Does not:
 * - Check legality
 * - Manage GameOver
 * - Manage Promotion
 * - Manage time
 */
class MoveExecutor
{

private:

    Board& board;



public:


    explicit MoveExecutor(Board& board);

    /**
     * @brief Executes an approved move.
     *
     * @return Information about what happened during the move.
     */
    MoveExecutionResult executeMove(const Position& from, const Position& to);

};