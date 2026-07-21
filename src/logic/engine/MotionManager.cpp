#include "MotionManager.hpp"
#include "GameConfig.hpp"
#include <cmath>
#include <algorithm>


bool MotionManager::startMotion(std::shared_ptr<Piece> piece, Position source, Position destination, int startTime)
{
    if(!piece)
        return false;

    if(activeMotion.has_value())
        return false;

    if(piece->getState() == PieceState::AIRBORNE)
        return false;

    if(piece->getState() == PieceState::LONG_REST || piece->getState() == PieceState::SHORT_REST)
    {
        return false;
    }

    int rowDistance = std::abs(destination.getRow() - source.getRow());
    int colDistance = std::abs(destination.getCol() - source.getCol());
    int distance = std::max(rowDistance, colDistance);
    int travelTime = distance * GameConfig::DEFAULT_TRAVEL_TIME_MS;

    piece->setState(PieceState::MOVING);

    activeMotion.emplace(piece, source, destination, startTime, travelTime);

    return true;
}

bool MotionManager::hasActiveMotion() const
{
    return activeMotion.has_value();
}

bool MotionManager::hasFinished(int currentTime) const
{
    if(!activeMotion)
        return false;

    return activeMotion->hasArrived(currentTime);
}

const Motion& MotionManager::getActiveMotion() const
{
    return activeMotion.value();
}

void MotionManager::clear()
{
    activeMotion.reset();
}

bool MotionManager::hasActiveMotionFor(std::shared_ptr<Piece> piece) const
{
    if(!activeMotion)
        return false;

    auto activePiece = activeMotion->getPiece();

    return activePiece && activePiece == piece;
}

Position MotionManager::getMotionStart(std::shared_ptr<Piece> piece) const
{
    if(hasActiveMotionFor(piece))
    {
        return activeMotion->getSource();
    }

    return Position(-1, -1);
}

Position MotionManager::getMotionDestination(std::shared_ptr<Piece> piece) const
{
    if(hasActiveMotionFor(piece))
    {
        return activeMotion->getDestination();
    }

    return Position(-1, -1);
}

int MotionManager::getMotionStartTime(std::shared_ptr<Piece> piece) const
{
    if(hasActiveMotionFor(piece))
    {
        return activeMotion->getStartTime();
    }

    return 0;
}

int MotionManager::getMotionFinishTime(std::shared_ptr<Piece> piece) const
{
    if(hasActiveMotionFor(piece))
    {
        return activeMotion->getFinishTime();
    }

    return 0;
}