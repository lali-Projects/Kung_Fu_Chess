#include "JumpManager.hpp"

//================================================
// Start Jump
//================================================
bool JumpManager::startJump(std::shared_ptr<Piece> piece, int startTime, int duration)
{
    if(!piece)
        return false;

    if(activeJump.has_value())
        return false;

    if(piece->getState() == PieceState::MOVING)
        return false;

    if(piece->getState() == PieceState::SHORT_REST || piece->getState() == PieceState::LONG_REST)
    {
        return false;
    }

    piece->setState(PieceState::AIRBORNE);

    activeJump.emplace(piece, piece->getPosition(), startTime, duration);

    return true;
}

//================================================
// Update
//================================================
bool JumpManager::update(int currentTime)
{
    if(!activeJump)
        return false;

    if(!activeJump->hasFinished(currentTime))
        return false;

    auto piece = activeJump->getPiece();

    if(piece)
    {
        finishedJumpPiece = piece;
    }

    activeJump.reset();

    return true;
}

//================================================
// Get Finished Jump Piece
//================================================
std::shared_ptr<Piece> JumpManager::getFinishedJumpPiece()
{
    auto temp = finishedJumpPiece;
    finishedJumpPiece.reset();
    return temp;
}

//================================================
// Queries
//================================================
bool JumpManager::hasActiveJump() const
{
    return activeJump.has_value();
}

bool JumpManager::hasActiveJumpFor(std::shared_ptr<Piece> piece) const
{
    if(!activeJump)
        return false;

    auto activePiece = activeJump->getPiece();

    return activePiece && activePiece == piece;
}

bool JumpManager::isJumpAt(const Position& pos) const
{
    if(!activeJump)
        return false;

    return activeJump->getPosition() == pos;
}

const Jump* JumpManager::getActiveJump() const
{
    if(!activeJump)
        return nullptr;

    return &activeJump.value();
}

int JumpManager::getStartTime(std::shared_ptr<Piece> piece) const
{
    if(!hasActiveJumpFor(piece))
        return 0;

    return activeJump->getStartTime();
}