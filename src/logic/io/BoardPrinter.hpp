#pragma once
#include "Board.hpp"
#include <iostream>

/**
 * @class BoardPrinter
 * @brief מחלקה האחראית על הדפסת לוח המשחק.
 * מספקת כלים סטטיים להדפסת ייצוג טקסטואלי קנוני של הלוח ומצב הכלים לזרם פלט מבוקש.
 */
class BoardPrinter {
public:
    /**
     * @brief מדפיס את מצב הלוח הנוכחי לזרם פלט (למשל std::cout).
     * @param board הפניה ללוח המשחק שיש להדפיס.
     * @param os זרם הפלט שאליו יודפס הלוח.
     */
    static void print(const Board& board, std::ostream& os);
};