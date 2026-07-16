#include "BoardInitializer.hpp"
#include "PieceEngine.hpp"

void BoardInitializer::setupInitialPosition(Board& board)
{
    PieceEngine::resetIdCounter();

    // White
    board.setPieceAt(7,0,PieceEngine::createPiece('R',Side::WHITE,{7,0}));
    board.setPieceAt(7,1,PieceEngine::createPiece('N',Side::WHITE,{7,1}));
    board.setPieceAt(7,2,PieceEngine::createPiece('B',Side::WHITE,{7,2}));
    board.setPieceAt(7,3,PieceEngine::createPiece('Q',Side::WHITE,{7,3}));
    board.setPieceAt(7,4,PieceEngine::createPiece('K',Side::WHITE,{7,4}));
    board.setPieceAt(7,5,PieceEngine::createPiece('B',Side::WHITE,{7,5}));
    board.setPieceAt(7,6,PieceEngine::createPiece('N',Side::WHITE,{7,6}));
    board.setPieceAt(7,7,PieceEngine::createPiece('R',Side::WHITE,{7,7}));

    for(int c = 0; c < 8; ++c)
        board.setPieceAt(6,c,
            PieceEngine::createPiece('P',Side::WHITE,{6,c}));

    // Black
    board.setPieceAt(0,0,PieceEngine::createPiece('R',Side::BLACK,{0,0}));
    board.setPieceAt(0,1,PieceEngine::createPiece('N',Side::BLACK,{0,1}));
    board.setPieceAt(0,2,PieceEngine::createPiece('B',Side::BLACK,{0,2}));
    board.setPieceAt(0,3,PieceEngine::createPiece('Q',Side::BLACK,{0,3}));
    board.setPieceAt(0,4,PieceEngine::createPiece('K',Side::BLACK,{0,4}));
    board.setPieceAt(0,5,PieceEngine::createPiece('B',Side::BLACK,{0,5}));
    board.setPieceAt(0,6,PieceEngine::createPiece('N',Side::BLACK,{0,6}));
    board.setPieceAt(0,7,PieceEngine::createPiece('R',Side::BLACK,{0,7}));

    for(int c = 0; c < 8; ++c)
        board.setPieceAt(1,c,
            PieceEngine::createPiece('P',Side::BLACK,{1,c}));
}
