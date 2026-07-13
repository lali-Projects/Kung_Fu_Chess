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

        // יצירת הכלי עם זמן התחלתי דינמי מהקונפיגורציה (במקום ה-0 שהיה קשיח)
        return std::make_shared<Piece>(GameConfig::INITIAL_TIME_MS, side, it->second, pos);
    }

    /**
     * @brief בדיקה האם סוג הכלי מוכר למנוע.
     * עונה על עיקרון ה-DRY - אין צורך לתחזק רשימת תווים נפרדת כאן.
     */
    static bool isKnownType(char type) {
        return GameConfig::TYPE_MAP.find(type) != GameConfig::TYPE_MAP.end();
    }
};