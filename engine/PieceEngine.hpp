#pragma once
#include "Piece.hpp"
#include "GameConfig.hpp"
#include <memory>
#include <string>

/**
 * @class PieceEngine
 * @brief מנוע יצירת הכלים.
 * אחראי על הפיכת נתונים גולמיים לאובייקטים של כלים בצורה דינמית (DRY).
 */
class PieceEngine {
public:
    /**
     * @brief יצירת כלי חדש על סמך סוג, צד ומיקום.
     * משתמש במפת הכלים הדינמית כדי למנוע כפילויות בקוד.
     */
    static std::shared_ptr<Piece> createPiece(char typeChar, Side side, Position pos) {
        // חיפוש סוג הכלי במפה הדינמית
        auto it = GameConfig::TYPE_MAP.find(typeChar);
        if (it == GameConfig::TYPE_MAP.end()) {
            return nullptr; // סוג כלי לא מוכר
        }

        // יצירת הכלי עם זמן התחלתי דינמי מהקונפיגורציה
        return std::make_shared<Piece>(GameConfig::INITIAL_TIME_MS, side, it->second, pos);
    }

    /**
     * @brief בדיקה האם סוג הכלי מוכר למנוע.
     */
    static bool isKnownType(char type) {
        return GameConfig::TYPE_MAP.find(type) != GameConfig::TYPE_MAP.end();
    }

    /**
     * @brief פונקציה חדשה: מקבלת טוקן גולמי (למשל "wK"), מאמתת אותו ומייצרת כלי.
     * @return std::shared_ptr<Piece> מצביע לכלי החדש, או nullptr אם הטוקן שגוי.
     */
    static std::shared_ptr<Piece> createFromToken(const std::string& token, const Position& pos) {
        
        if (token.length() != 2) {
            return nullptr;
        }
        
        char sideChar = token[0];  // התו הראשון מייצג את הצד (w / b)
        char typeChar = token[1];  // התו השני מייצג את סוג הכלי (K, Q, R...)
        Side side;
        
       
        if (sideChar == GameConfig::SIDE_WHITE) {
            side = Side::WHITE;
        } else if (sideChar == GameConfig::SIDE_BLACK) {
            side = Side::BLACK;
        } else {
            return nullptr; // אם זה לא 'w' ולא 'b' - הטוקן אינו חוקי!
        }

        // 3. שימוש בפונקציה הקיימת ליצירת הכלי בפועל (כולל בדיקת סוג הכלי)
        return createPiece(typeChar, side, pos);
    }
};