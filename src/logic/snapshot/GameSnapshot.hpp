#pragma once

#include <vector>
#include <optional>

#include "PieceSnapshot.hpp"
#include "Position.hpp"

/**
 * @class GameSnapshot
 * @brief מייצגת "צילום מצב" של המשחק ברגע נתון.
 * 
 * מחלקה זו משמשת כ-Data Transfer Object (DTO). היא מאגדת את הנתונים 
 * הנדרשים לתצוגה בצורה קריאה בלבד, כדי להפריד בין הלוגיקה הפנימית 
 * של המנוע לבין שכבת ה-GUI.
 */
class GameSnapshot
{
private:
    std::vector<PieceSnapshot> pieces;

    int currentTimeMs;
   
    bool gameOver;

    std::optional<Position> selectedPosition;

public:
    /**
     * @brief בנאי המאתחל את מצב המשחק.
     */
    GameSnapshot(
        const std::vector<PieceSnapshot>& pieces,
        int currentTimeMs,
        bool gameOver,
        std::optional<Position> selectedPosition = std::nullopt);

    /**
     * @brief מחזיר את רשימת הכלים הקיימים ב-Snapshot.
     */
    const std::vector<PieceSnapshot>& getPieces() const;

    /**
     * @brief מחזיר את זמן המשחק השמור.
     */
    int getCurrentTime() const;

    /**
     * @brief בודק האם המצב המצולם הוא מצב של סיום משחק.
     */
    bool isGameOver() const;

    /**
     * @brief מחזיר את המיקום שנבחר (אם נבחר).
     */
    const std::optional<Position>& getSelectedPosition() const;

    /**
     * @brief מעדכן את המיקום הנבחר בתוך ה-Snapshot.
     */
    void setSelectedPosition(
        const std::optional<Position>& position);
};