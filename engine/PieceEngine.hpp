#pragma once
#include "Piece.hpp"
#include "GameConfig.hpp"
#include <memory>
#include <string>
#include <atomic> // שומר על ה-ID ייחודי ובטוח גם בסביבה מרובת תהליכונים

/**
 * @class PieceEngine
 * @brief מנוע יצירת הכלים.
 * אחראי על הפיכת נתונים גולמיים לאובייקטים של כלים בצורה דינמית (DRY) והקצאת מזהים ייחודיים.
 */
class PieceEngine {
private:
    // מונה סטטי שמתחיל מ-1. השימוש ב-inline מאפשר לאתחל אותו ישירות בתוך ה-Header (החל מ-C++17).
    inline static std::atomic<int> nextId{1};

public:
    /**
     * @brief יצירת כלי חדש על סמך סוג, צד ומיקום.
     * משתמש במפת הכלים הדינמית ומקצה מזהה (ID) ייחודי רציף ואוטומטי.
     */
    static std::shared_ptr<Piece> createPiece(char typeChar, Side side, Position pos) {
        // חיפוש סוג הכלי במפה הדינמית
        auto it = GameConfig::TYPE_MAP.find(typeChar);
        if (it == GameConfig::TYPE_MAP.end()) {
            return nullptr; // סוג כלי לא מוכר
        }

        // הפקת מזהה ייחודי: לוקח את הערך הנוכחי ומקדם את המונה עבור הכלי הבא
        int currentId = nextId++;

        // יצירת הכלי עם ה-ID האוטומטי (מתקן את השגיאה שבה הועבר הזמן ההתחלתי)
        return std::make_shared<Piece>(currentId, side, it->second, pos);
    }

    /**
     * @brief בדיקה האם סוג הכלי מוכר למנוע.
     */
    static bool isKnownType(char type) {
        return GameConfig::TYPE_MAP.find(type) != GameConfig::TYPE_MAP.end();
    }

    /**
     * @brief פונקציה שמקבלת טוקן גולמי (למשל "wK"), מאמתת אותו ומייצרת כלי עם ID אוטומטי.
     * @return std::shared_ptr<Piece> מצביע לכלי החדש, או nullptr אם הטוקן שגוי.
     */
    static std::shared_ptr<Piece> createFromToken(const std::string& token, const Position& pos) {
        
        if (token.length() != 2) {
            return nullptr;
        }
        
        char sideChar = token[0];  // התו הראשון מייצג את הצד (w / b)
        char typeChar = token[1];  // התו השני מייצג את סוג הכלי (K, Q, R...)
        Side side;
        
        // אימות קפדני של הצד למניעת באגים לוגיים
        if (sideChar == GameConfig::SIDE_WHITE) {
            side = Side::WHITE;
        } else if (sideChar == GameConfig::SIDE_BLACK) {
            side = Side::BLACK;
        } else {
            return nullptr; // אם זה לא 'w' ולא 'b' - הטוקן אינו חוקי!
        }

        // קריאה ל-createPiece שמטפלת בהצלבה מול המפה ובשליפת ה-ID הייחודי
        return createPiece(typeChar, side, pos);
    }

    /**
     * @brief מאפס את מונה המזהים חזרה ל-1.
     * חובה לקרוא לפונקציה זו כאשר מאתחלים לוח חדש או מתחילים משחק מאפס!
     */
    static void resetIdCounter() {
        nextId = 1;
    }
};