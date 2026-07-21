#include "RestManager.hpp"
#include "GameConfig.hpp"


bool RestManager::startRest(
    std::shared_ptr<Piece> piece,
    PieceState restState,
    PieceState nextState,
    int startTime)
{
    if(!piece)
        return false;


    int duration = getRestDuration(restState);


    piece->setState(restState);


    activeRests.emplace_back(
        piece,
        restState,
        nextState,
        startTime,
        duration
    );


    return true;
}


int RestManager::getRestDuration(PieceState restState) const
{
    switch(restState)
    {
        case PieceState::LONG_REST:
            return GameConfig::LONG_REST_DURATION_MS;

        case PieceState::SHORT_REST:
            return GameConfig::SHORT_REST_DURATION_MS;

        default:
            return 0;
    }
}


void RestManager::update(int currentTime)
{
    for(auto it = activeRests.begin(); it != activeRests.end();)
    {
        if(it->hasFinished(currentTime))
        {
            auto piece = it->getPiece();

            if(piece)
            {
                piece->setState(it->getNextState());
            }

            it = activeRests.erase(it);
        }
        else
        {
            ++it;
        }
    }
}


bool RestManager::hasActiveRestFor(std::shared_ptr<Piece> piece) const
{
    for(const auto& rest : activeRests)
    {
        auto activePiece = rest.getPiece();

        if(activePiece && activePiece == piece)
        {
            return true;
        }
    }

    return false;
}


int RestManager::getRestStartTime(std::shared_ptr<Piece> piece) const
{
    for(const auto& rest : activeRests)
    {
        auto activePiece = rest.getPiece();

        if(activePiece && activePiece == piece)
        {
            return rest.getStartTime();
        }
    }

    return 0;
}


int RestManager::getRestFinishTime(std::shared_ptr<Piece> piece) const
{
    for(const auto& rest : activeRests)
    {
        auto activePiece = rest.getPiece();

        if(activePiece && activePiece == piece)
        {
            return rest.getStartTime() + rest.getDuration();
        }
    }

    return 0;
}