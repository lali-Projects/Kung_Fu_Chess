#include "SnapshotBuilder.hpp"

#include "GameEngine.hpp"
#include "Board.hpp"
#include "Piece.hpp"

GameSnapshot SnapshotBuilder::build(
    const GameEngine& engine)
{
    GameSnapshot snapshot;

    snapshot.setCurrentTime(
        engine.getCurrentTime());

    snapshot.setGameOver(
        engine.gameOver());

    const Board& board =
        engine.getBoard();

    for (int row = 0; row < board.getRows(); ++row)
    {
        for (int col = 0; col < board.getCols(); ++col)
        {
            auto piece =
                board.getPieceAt(
                    Position(row, col));

            if (!piece)
            {
                continue;
            }

            snapshot.addPiece(
                PieceSnapshot(
                    piece->getId(),
                    piece->getSide(),
                    piece->getType(),
                    piece->getState(),
                    piece->getPosition()));
        }
    }

    return snapshot;
}