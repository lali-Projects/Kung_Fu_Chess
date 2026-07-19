#pragma once
#include <unordered_map>
#include "Piece.hpp" 

/**
 * @class GameConfig
 * @brief מחלקת הגדרות מרכזית. כל המשתנים ניתנים לשינוי דינמי בזמן ריצה.
 */
class GameConfig {
public:
    // הגדרות זמנים ומידות
    static int CELL_SIZE_PX;
    static int DEFAULT_TRAVEL_TIME_MS;
    static int INITIAL_TIME_MS;

    // משכי מנוחה (Rest) לאחר פעולות בזמן אמת.
    // אלו קבועי משחק (gameplay), נפרדים במתכוון ממשכי האנימציה שב-JSON.
    static int LONG_REST_DURATION_MS;   // מנוחה לאחר תנועה רגילה (Motion)
    static int SHORT_REST_DURATION_MS;  // מנוחה לאחר קפיצה (Jump)

    // הגדרות תווים ופורמט (כעת כמשתנים רגילים)
    static char EMPTY_CELL;
    static char SIDE_WHITE;
    static char SIDE_BLACK;

    // מפת הכלים (כעת כמשתנה רגיל שניתן לעדכן בזמן ריצה)
    static std::unordered_map<char, PieceType> TYPE_MAP;

};