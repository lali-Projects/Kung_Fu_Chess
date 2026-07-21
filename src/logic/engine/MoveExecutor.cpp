#include "MoveExecutor.hpp"

MoveExecutor::MoveExecutor(Board& b) : board(b)
{
}

MoveExecutionResult MoveExecutor::executeMove(const Position& from, const Position& to)
{
    MoveExecutionResult result;

    auto movingPiece = board.getPieceAt(from);

    if(!movingPiece)
    {
        return result;
    }

    result.movingPiece = movingPiece;

    auto capturedPiece = board.getPieceAt(to);

    if(capturedPiece)
    {
        result.wasCapture = true;
        result.capturedPiece = capturedPiece;
        board.removePiece(to);
    }

    board.movePiece(from, to);

    return result;
}