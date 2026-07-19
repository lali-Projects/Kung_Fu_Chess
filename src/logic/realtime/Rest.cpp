#include "Rest.hpp"


Rest::Rest(
    std::shared_ptr<Piece> piece,
    PieceState state,
    PieceState nextState,
    int startTime,
    int duration)
:
piece(piece),
state(state),
nextState(nextState),
startTime(startTime),
duration(duration)
{
}



bool Rest::hasFinished(int currentTime) const
{
    std::cout
    << "HAS FINISHED CHECK current="
    << currentTime
    << " end="
    << startTime + duration
    << std::endl;


    return currentTime >= startTime + duration;
}



std::shared_ptr<Piece> Rest::getPiece() const
{
    return piece;
}



PieceState Rest::getState() const
{
    return state;
}



PieceState Rest::getNextState() const
{
    return nextState;
}



int Rest::getStartTime() const
{
    return startTime;
}



int Rest::getDuration() const
{
    return duration;
}