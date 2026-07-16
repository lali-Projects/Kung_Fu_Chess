#pragma once
#include "Board.hpp"
#include "Position.hpp"
#include "GameEngine.hpp"
#include "BoardMapper.hpp"
#include <optional>

/**
 * @class GameController
 * @brief בקר המשחק (Controller).
 * אחראי על קבלת קלטים מהמשתמש (למשל לחיצות עכבר), תרגומם לקואורדינטות הלוח,
 * ופנייה למנוע המשחק (GameEngine) לביצוע הלוגיקה והמהלכים.
 */
class GameController {
private:
    Board& board;
    GameEngine& gameEngine;
    std::optional<Position> selectedPosition;

public:
    /**
     * @brief בנאי המאתחל את הבקר עם הפניות ללוח ולמנוע המשחק.
     */
    GameController(Board& b, GameEngine& ge);

    /**
     * @brief מבצע פעולת קפיצה (Jump) עבור כלי בהתבסס על מיקום פיקסלים במסך.
     * @param pixelX מיקום ה-X בפיקסלים.
     * @param pixelY מיקום ה-Y בפיקסלים.
     */
    MoveResult jump(const Position& pos);

    /**
     * @brief מטפל בלחיצה (Click) על המסך - בחירת כלי, החלפת בחירה או ביצוע מהלך.
     * @param pixelX מיקום ה-X בפיקסלים.
     * @param pixelY מיקום ה-Y בפיקסלים.
     */
    MoveResult click(const Position& pos);

    /**
     * @brief מאפס את הבחירה הנוכחית של המשבצת.
     */
    void clearSelection();

    /**
     * @brief מחזיר את המיקום הנבחר כרגע (אם קיים).
     * @return std::optional<Position> המיקום הנבחר או std::nullopt אם אין בחירה.
     */
    const std::optional<Position>& getSelectedPosition() const;
};