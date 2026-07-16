#include "GameEngine.hpp"
#include "GameSnapshot.hpp"

MoveResult GameEngine::requestMove(const Position& from, const Position& to) {
    if (isGameOver) return {false, "game_over"};
    if (realTimeArbiter.hasActiveMotion()) return {false, "MOTION_IN_PROGRESS"};

    MoveValidation validation = ruleEngine.isValidMove(to, from, board);
    if (!validation.is_valid) return {false, validation.reason};

    // השינוי: הארביטר מחשב את הזמן בעצמו, המנוע רק פוקד עליו להתחיל
    realTimeArbiter.startMotion(board.getPieceAt(from), from, to, currentTimeMs);
    
    return {true, "ok"};
}

MoveResult GameEngine::requestJump(const Position& position) {
    if (isGameOver) return { false, "game_over" };

    std::shared_ptr<Piece> selectedPiece = board.getPieceAt(position);
    if (selectedPiece == nullptr) return { false, "empty_source" };

    MoveValidation validation = ruleEngine.isValidJump(selectedPiece);
    if (!validation.is_valid) return { false, validation.reason };

    realTimeArbiter.startJump(selectedPiece, currentTimeMs, GameConfig::DEFAULT_TRAVEL_TIME_MS);
    return { true, "ok" };
}

GameSnapshot GameEngine::getSnapshot() const
{
    std::vector<PieceSnapshot> pieces;


    for(int row = 0;
        row < board.getRows();
        row++)
    {
        for(int col = 0;
            col < board.getCols();
            col++)
        {

            Position pos(
                row,
                col);


            auto piece =
                board.getPieceAt(pos);


            if(piece)
            {
                pieces.push_back(
                    PieceSnapshot
                    {
                        piece->getId(),
                        piece->getSide(),
                        piece->getType(),
                        piece->getPosition(),
                        piece->getState()
                    });
            }
        }
    }


    return GameSnapshot(
        pieces,
        currentTimeMs,
        isGameOver);
}
