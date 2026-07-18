#pragma once

#include "Img.hpp"
#include "Layout.hpp"
#include "TextureManager.hpp"
#include "BoardRenderer.hpp"
#include "SelectionRenderer.hpp"
#include "PieceRenderer.hpp"

/**
 * @class GameRenderer
 * @brief מנהל הרינדור המרכזי של המשחק.
 *
 * מחלקה זו מרכזת את כל רכיבי הציור הספציפיים (לוח, כלים, בחירה) ומספקת 
 * ממשק אחיד ופשוט לרינדור מצב המשחק השלם על גבי הקנבס.
 */
class GameRenderer
{
private:
    BoardRenderer boardRenderer;       
    PieceRenderer pieceRenderer;         
    SelectionRenderer selectionRenderer; 

public:
    /**
     * @brief בנאי המאתחל את כל רכיבי הרינדור הפנימיים.
     * @param layout אובייקט ה-Layout להגדרת מידות ומיקומי אלמנטים על המסך.
     * @param textureManager מנהל הטקסטורות המספק את המשאבים הגרפיים הנדרשים.
     */
    GameRenderer(const Layout& layout, TextureManager& textureManager);
    /**
     * @brief מבצע את תהליך הציור המלא של סצנת המשחק.
     * 
     * פונקציה זו מתאמת את סדר הציור: תחילה הלוח, לאחר מכן הכלים, ולבסוף סימוני הבחירה,
     * כדי להבטיח שכל אלמנט יופיע בשכבה הנכונה.
     *
     * @param canvas המשטח הגרפי שעליו יתבצע הציור.
     * @param snapshot מצב המשחק הנוכחי המכיל את מיקומי הכלים והבחירה של המשתמש.
     */
    void render(Img& canvas, const GameSnapshot& snapshot);
};