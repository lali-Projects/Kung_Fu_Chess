#include <iostream>

#include "Board.hpp"
#include "PieceEngine.hpp"
#include "RuleEngine.hpp"
#include "RealTimeArbiter.hpp"
#include "GameEngine.hpp"

#include "SnapshotBuilder.hpp"

int main()
{
    //----------------------------------------------------
    // Reset IDs
    //----------------------------------------------------

    PieceEngine::resetIdCounter();

    //----------------------------------------------------
    // Create board
    //----------------------------------------------------

    Board board(8, 8);

    //----------------------------------------------------
    // Create pieces
    //----------------------------------------------------

    auto whiteKing =
        PieceEngine::createFromToken(
            "wK",
            Position(7, 4));

    auto blackKing =
        PieceEngine::createFromToken(
            "bK",
            Position(0, 4));

    auto whitePawn =
        PieceEngine::createFromToken(
            "wP",
            Position(6, 4));

    //----------------------------------------------------
    // Put pieces on board
    //----------------------------------------------------

    board.setPieceAt(7, 4, whiteKing);
    board.setPieceAt(0, 4, blackKing);
    board.setPieceAt(6, 4, whitePawn);

    //----------------------------------------------------
    // Create game objects
    //----------------------------------------------------

    RuleEngine ruleEngine;

    RealTimeArbiter arbiter(board);

    GameEngine engine(
        board,
        ruleEngine,
        arbiter);

    //----------------------------------------------------
    // Build snapshot
    //----------------------------------------------------

    GameSnapshot snapshot =
        SnapshotBuilder::build(engine);

    //----------------------------------------------------
    // Print snapshot
    //----------------------------------------------------

    std::cout << "==============================\n";
    std::cout << " SNAPSHOT TEST\n";
    std::cout << "==============================\n\n";

    std::cout
        << "Current Time : "
        << snapshot.getCurrentTime()
        << "\n";

    std::cout
        << "Game Over   : "
        << std::boolalpha
        << snapshot.isGameOver()
        << "\n";

    std::cout
        << "Pieces      : "
        << snapshot.getPieces().size()
        << "\n\n";

    for (const PieceSnapshot& piece : snapshot.getPieces())
    {
        std::cout
            << "ID: "
            << piece.getId()

            << "   "

            << piece.toCanonicalString()

            << "   "

            << "Row: "
            << piece.getPosition().getRow()

            << "   "

            << "Col: "
            << piece.getPosition().getCol()

            << "   "

            << "State: ";

        switch (piece.getState())
        {
            case PieceState::IDLE:
                std::cout << "IDLE";
                break;

            case PieceState::MOVING:
                std::cout << "MOVING";
                break;

            case PieceState::AIRBORNE:
                std::cout << "AIRBORNE";
                break;

            case PieceState::CAPTURED:
                std::cout << "CAPTURED";
                break;
        }

        std::cout << '\n';
    }

    std::cout << "\nSnapshot build succeeded.\n";

    return 0;
}