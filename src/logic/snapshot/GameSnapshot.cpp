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



//---------------------------------
// Pieces
//---------------------------------


const std::vector<PieceSnapshot>& 
GameSnapshot::getPieces() const
{
    return pieces;
}



std::vector<PieceSnapshot>& 
GameSnapshot::getMutablePieces()
{
    return pieces;
}



//---------------------------------
// Time
//---------------------------------


int GameSnapshot::getCurrentTime() const
{
    return currentTimeMs;
}



//---------------------------------
// Game state
//---------------------------------


bool GameSnapshot::isGameOver() const
{
    return gameOver;
}



//---------------------------------
// Selection
//---------------------------------


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