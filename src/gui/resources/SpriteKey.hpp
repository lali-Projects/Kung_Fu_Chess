#pragma once

#include <string>
#include "PieceSnapshot.hpp"
/**
 * @class SpriteKey
 * @brief Sprite ממירה את נתוני הכלי למחרוזת המשמשת כמפתח ל־ .
 */
class SpriteKey
{
public:
    /**
     * @brief יוצר מפתח טקסטואלי המבוסס על מצב הכלי.
     */
    static std::string create(const PieceSnapshot& piece);

private:
    /**
     * @brief פונקציית עזר להמרת מצב הכלי למחרוזת.
     */
    static std::string stateToString(PieceState state);
};