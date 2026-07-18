#pragma once

#include "Img.hpp"
#include "Layout.hpp"
#include "GameSnapshot.hpp"

/**
 * @class SelectionRenderer
 * @brief מחלקה האחראית על מתן משוב ויזואלי לבחירת המשתמש.
 *
 * מחלקה זו אחראית לזהות את המיקום שנבחר על ידי המשתמש מתוך מצב המשחק (snapshot),
 * ולצייר חיווי גרפי (כמו מסגרת) על גבי הלוח כדי להדגיש את הבחירה.
 */
class SelectionRenderer
{
private:
    /// הפניה ל-Layout המשמש לחישוב הקואורדינטות של תאי הלוח.
    const Layout& layout;

public:
    /**
     * @brief בנאי המחלקה.
     * @param layout הפניה לאובייקט ה-Layout המנהל את מידות ומיקומי הלוח.
     */
    SelectionRenderer(const Layout& layout);

    /**
     * @brief מבצעת פעולת ציור של סימון הבחירה על גבי הקנבס.
     *
     * פונקציה זו בודקת האם קיימת משבצת נבחרת ב-snapshot. אם כן, היא מחשבת את
     * המיקום הגיאומטרי שלה באמצעות ה-Layout ומציירת מסגרת הדגשה.
     *
     * @param canvas המשטח הגרפי עליו מתבצע הציור.
     * @param snapshot מצב המשחק הנוכחי המכיל את מיקום הבחירה.
     */
    void draw(Img& canvas, const GameSnapshot& snapshot) const;
};