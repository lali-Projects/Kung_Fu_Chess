#include "GameEngine.hpp"
#include "GameSnapshot.hpp"

GameEngine::GameEngine(Board& b, RuleEngine& rules, RealTimeArbiter& arbiter)
: board(b),
  ruleEngine(rules),
  realTimeArbiter(arbiter),
  currentTimeMs(GameConfig::INITIAL_TIME_MS),
  isGameOver(false)
{
}

//================================================
// Request Move
//================================================
MoveResult GameEngine::requestMove(const Position& from, const Position& to)
{
    if(isGameOver)
    {
        return {
            false,
            "game_over"
        };
    }

    if(realTimeArbiter.hasActiveMotion())
    {
        return {
            false,
            "MOTION_IN_PROGRESS"
        };
    }

    MoveValidation validation = ruleEngine.isValidMove(to, from, board);

    if(!validation.is_valid)
    {
        return {
            false,
            validation.reason
        };
    }

    auto piece = board.getPieceAt(from);

    realTimeArbiter.startMotion(piece, from, to, currentTimeMs);

    return {
        true,
        "ok"
    };
}

//================================================
// Request Jump
//================================================
MoveResult GameEngine::requestJump(const Position& position)
{
    if(isGameOver)
    {
        return {
            false,
            "game_over"
        };
    }

    auto piece = board.getPieceAt(position);

    if(!piece)
    {
        return {
            false,
            "empty_source"
        };
    }

    MoveValidation validation = ruleEngine.isValidJump(piece);

    if(!validation.is_valid)
    {
        return {
            false,
            validation.reason
        };
    }

    realTimeArbiter.startJump(piece, currentTimeMs, GameConfig::DEFAULT_TRAVEL_TIME_MS);

    return {
        true,
        "ok"
    };
}


void GameEngine::handleMoveExecutionResult(const MoveExecutionResult& result)
{
   
    if(result.wasCapture && result.capturedPiece)
    {
        if(gameOverHandler.isGameOver(*result.capturedPiece))
        {
            signalGameOver();
        }
    }

   
    if(result.movingPiece)
    {
        promotionHandler.handlePromotion(*result.movingPiece, board);
    }
}

//================================================
// Time
//================================================
void GameEngine::wait(int milliseconds)
{
    currentTimeMs += milliseconds;

    realTimeArbiter.advanceTime(currentTimeMs, *this);
}

//================================================
// State
//================================================
void GameEngine::signalGameOver()
{
    isGameOver = true;
}

bool GameEngine::gameOver() const
{
    return isGameOver;
}

const Board& GameEngine::getBoard() const
{
    return board;
}

int GameEngine::getCurrentTime() const
{
    return currentTimeMs;
}

//================================================
// Snapshot
//================================================
GameSnapshot GameEngine::getSnapshot() const
{
    std::vector<PieceSnapshot> pieces;

    for(int row = 0; row < board.getRows(); row++)
    {
        for(int col = 0; col < board.getCols(); col++)
        {
            Position pos(row, col);

            auto piece = board.getPieceAt(pos);

            if(!piece)
                continue;

            bool hasMotion = realTimeArbiter.hasActiveMotionFor(piece);

            bool hasAnimation = realTimeArbiter.hasActiveAnimation(piece);

            pieces.push_back(
            {
                piece->getId(),
                piece->getSide(),
                piece->getType(),
                piece->getPosition(),
                piece->getState(),
                realTimeArbiter.getAnimationStartTime(piece),
                hasAnimation,
                hasMotion,
                hasMotion ? realTimeArbiter.getMotionStart(piece) : piece->getPosition(),
                hasMotion ? realTimeArbiter.getMotionDestination(piece) : piece->getPosition(),
                hasMotion ? realTimeArbiter.getMotionStartTime(piece) : 0,
                hasMotion ? realTimeArbiter.getMotionFinishTime(piece) : 0
            });
        }
    }

    return GameSnapshot(pieces, currentTimeMs, isGameOver);
}

int GameEngine::getAnimationStartTime(int pieceId) const
{
    auto piece = getPieceById(pieceId);

    if(!piece)
    {
        return 0;
    }

    return realTimeArbiter.getAnimationStartTime(piece);
}

std::shared_ptr<Piece> GameEngine::getPieceById(int pieceId) const
{
    for(int row = 0; row < board.getRows(); row++)
    {
        for(int col = 0; col < board.getCols(); col++)
        {
            auto piece =
                board.getPieceAt(Position(row, col));

            if(piece && piece->getId() == pieceId)
            {
                return piece;
            }
        }
    }

    return nullptr;
}