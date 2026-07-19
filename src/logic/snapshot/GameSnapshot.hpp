#pragma once

#include <vector>
#include <optional>

#include "PieceSnapshot.hpp"
#include "Position.hpp"


/**
 * @class GameSnapshot
 *
 * @brief צילום מצב לקריאה עבור שכבת GUI.
 *
 * אינו משנה את המשחק.
 * אינו מכיל לוגיקה.
 */
class GameSnapshot
{

private:

    std::vector<PieceSnapshot> pieces;

    int currentTimeMs;

    bool gameOver;

    std::optional<Position> selectedPosition;



public:


    GameSnapshot(
        const std::vector<PieceSnapshot>& pieces,
        int currentTimeMs,
        bool gameOver,
        std::optional<Position> selectedPosition = std::nullopt);



    //---------------------------------
    // Pieces
    //---------------------------------


    const std::vector<PieceSnapshot>& getPieces() const;



    /**
     * מאפשר ל־Builder להשלים מידע לתצוגה.
     */
    std::vector<PieceSnapshot>& getMutablePieces();



    //---------------------------------
    // Time
    //---------------------------------


    int getCurrentTime() const;



    //---------------------------------
    // Game state
    //---------------------------------


    bool isGameOver() const;



    //---------------------------------
    // Selection
    //---------------------------------


    const std::optional<Position>& 
    getSelectedPosition() const;



    void setSelectedPosition(
        const std::optional<Position>& position);

};