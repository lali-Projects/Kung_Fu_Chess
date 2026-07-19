#include "GameEngine.hpp"
#include "GameSnapshot.hpp"

MoveResult GameEngine::requestMove(const Position& from, const Position& to) 
{
    if (isGameOver) 
    {
        return {false, "game_over"};
    }
    
    if (realTimeArbiter.hasActiveMotion()) 
    {
        return {false, "MOTION_IN_PROGRESS"};
    }

    MoveValidation validation = ruleEngine.isValidMove(to, from, board);
    if (!validation.is_valid) 
    {
        return {false, validation.reason};
    }

    
    realTimeArbiter.startMotion(board.getPieceAt(from), from, to, currentTimeMs);
    
    return {true, "ok"};
}

MoveResult GameEngine::requestJump(const Position& position) 
{
    if (isGameOver) 
    {
        return {false, "game_over"};
    }

    std::shared_ptr<Piece> selectedPiece = board.getPieceAt(position);
    if (selectedPiece == nullptr) 
    {
        return {false, "empty_source"};
    }

    MoveValidation validation = ruleEngine.isValidJump(selectedPiece);
    if (!validation.is_valid) 
    {
        return {false, validation.reason};
    }

    realTimeArbiter.startJump(selectedPiece, currentTimeMs, GameConfig::DEFAULT_TRAVEL_TIME_MS);
    return {true, "ok"};
}

GameSnapshot GameEngine::getSnapshot() const
{
    std::vector<PieceSnapshot> pieces;

    for (int row = 0; row < board.getRows(); row++)
    {
        for (int col = 0; col < board.getCols(); col++)
        {
            Position pos(row, col);
            auto piece = board.getPieceAt(pos);

            if (!piece)
            {
                continue;
            }

            int id = piece->getId();
            bool hasMotion = realTimeArbiter.hasActiveMotionFor(id);
            bool hasAnimation = realTimeArbiter.hasActiveAnimation(id);

            PieceSnapshot snapshot{
                id,
                piece->getSide(),
                piece->getType(),
                piece->getPosition(),
                piece->getState(),
                
                // Animation
                realTimeArbiter.getAnimationStartTime(id),
                hasAnimation,
                
                // Motion
                hasMotion,
                hasMotion ? realTimeArbiter.getMotionStart(id) : piece->getPosition(),
                hasMotion ? realTimeArbiter.getMotionDestination(id) : piece->getPosition(),
                hasMotion ? realTimeArbiter.getMotionStartTime(id) : 0,
                hasMotion ? realTimeArbiter.getMotionFinishTime(id) : 0
            };

            pieces.push_back(snapshot);
        }
    }

    return GameSnapshot(pieces, currentTimeMs, isGameOver);
}

int GameEngine::getAnimationStartTime(int pieceId) const
{
    return realTimeArbiter.getAnimationStartTime(pieceId);
}