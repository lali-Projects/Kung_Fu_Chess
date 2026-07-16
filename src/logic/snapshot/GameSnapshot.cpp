#include "GameSnapshot.hpp"


GameSnapshot::GameSnapshot(
    const std::vector<PieceSnapshot>& pieces,
    int currentTimeMs,
    bool gameOver,
    std::optional<Position> selectedPosition)
:
pieces(pieces),
currentTimeMs(currentTimeMs),
gameOver(gameOver),
selectedPosition(selectedPosition)
{
}


const std::vector<PieceSnapshot>& 
GameSnapshot::getPieces() const
{
    return pieces;
}


int GameSnapshot::getCurrentTime() const
{
    return currentTimeMs;
}


bool GameSnapshot::isGameOver() const
{
    return gameOver;
}


const std::optional<Position>&
GameSnapshot::getSelectedPosition() const
{
    return selectedPosition;
}


void GameSnapshot::setSelectedPosition(
    const std::optional<Position>& position)
{
    selectedPosition = position;
}