#include "RealTimeArbiter.hpp"
#include "GameEngine.hpp"

//================================================
// Constructor
//================================================
RealTimeArbiter::RealTimeArbiter(Board& b)
: board(b),
  motionManager(),
  jumpManager(),
  restManager(),
  moveExecutor(b)
{
}

//================================================
// Time
//================================================
void RealTimeArbiter::advanceTime(int currentTime, GameEngine& engine)
{
    //---------------------------------
    // Motion
    //---------------------------------
    if(motionManager.hasActiveMotion() && motionManager.hasFinished(currentTime))
    {
        processMotionCompletion(engine);
    }

    //---------------------------------
    // Jump
    //---------------------------------
    bool jumpFinished = jumpManager.update(currentTime);

    if(jumpFinished)
    {
        auto piece = jumpManager.getFinishedJumpPiece();

        if(piece)
        {
            restManager.startRest(
                piece,
                PieceState::SHORT_REST,
                PieceState::IDLE,
                currentTime
            );
        }
    }

    //---------------------------------
    // Rest
    //---------------------------------
    restManager.update(currentTime);
}

//================================================
// Motion
//================================================
bool RealTimeArbiter::startMotion(std::shared_ptr<Piece> piece, Position from, Position to, int startTime)
{
    return motionManager.startMotion(piece, from, to, startTime);
}

void RealTimeArbiter::processMotionCompletion(GameEngine& engine)
{
    const Motion& motion = motionManager.getActiveMotion();

    //---------------------------------
    // Execute physical move
    //---------------------------------
    MoveExecutionResult result = moveExecutor.executeMove(motion.getSource(), motion.getDestination());

    //---------------------------------
    // Notify GameEngine
    //---------------------------------
    engine.handleMoveExecutionResult(result);

    //---------------------------------
    // Start rest animation
    //---------------------------------
    auto piece = result.movingPiece;

    if(piece)
    {
        restManager.startRest(
            piece,
            PieceState::LONG_REST,
            PieceState::IDLE,
            motion.getFinishTime()
        );
    }

    motionManager.clear();
}

bool RealTimeArbiter::hasActiveMotion() const
{
    return motionManager.hasActiveMotion();
}

//================================================
// Jump
//================================================
bool RealTimeArbiter::startJump(std::shared_ptr<Piece> piece, int startTime, int duration)
{
    return jumpManager.startJump(piece, startTime, duration);
}

bool RealTimeArbiter::hasActiveJump() const
{
    return jumpManager.hasActiveJump();
}

//================================================
// Rest
//================================================
bool RealTimeArbiter::hasActiveRestFor(std::shared_ptr<Piece> piece) const
{
    return restManager.hasActiveRestFor(piece);
}

//================================================
// Animation
//================================================
bool RealTimeArbiter::hasActiveAnimation(std::shared_ptr<Piece> piece) const
{
    return motionManager.hasActiveMotionFor(piece)
        || jumpManager.hasActiveJumpFor(piece)
        || restManager.hasActiveRestFor(piece);
}

int RealTimeArbiter::getAnimationStartTime(std::shared_ptr<Piece> piece) const
{
    if(motionManager.hasActiveMotionFor(piece))
    {
        return motionManager.getMotionStartTime(piece);
    }

    if(jumpManager.hasActiveJumpFor(piece))
    {
        return jumpManager.getStartTime(piece);
    }

    if(restManager.hasActiveRestFor(piece))
    {
        return restManager.getRestStartTime(piece);
    }

    return 0;
}

//================================================
// Motion Queries
//================================================
bool RealTimeArbiter::hasActiveMotionFor(std::shared_ptr<Piece> piece) const
{
    return motionManager.hasActiveMotionFor(piece);
}

Position RealTimeArbiter::getMotionStart(std::shared_ptr<Piece> piece) const
{
    return motionManager.getMotionStart(piece);
}

Position RealTimeArbiter::getMotionDestination(std::shared_ptr<Piece> piece) const
{
    return motionManager.getMotionDestination(piece);
}

int RealTimeArbiter::getMotionStartTime(std::shared_ptr<Piece> piece) const
{
    return motionManager.getMotionStartTime(piece);
}

int RealTimeArbiter::getMotionFinishTime(std::shared_ptr<Piece> piece) const
{
    return motionManager.getMotionFinishTime(piece);
}