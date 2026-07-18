#pragma once

#include "Img.hpp"
#include "Layout.hpp"
#include "TextureManager.hpp"
#include "GameSnapshot.hpp"

/**
 * @class PieceRenderer
 * @brief מחלקה האחראית על רינדור ותצוגה ויזואלית של רכיבים על גבי משטח גרפי.
 * 
 * מחלקה זו מתרגמת את מצב המערכת הנתון לייצוג גרפי באמצעות טקסטורות ומיקומי פריסה.
 */
class PieceRenderer
{
private:
    const Layout& layout;         
    TextureManager& textureManager;    

public:
    /**
     * @brief בנאי המחלקה.
     * @param layout אובייקט האחראי על חישוב וניהול המיקומים (Layout).
     * @param textureManager אובייקט האחראי על ניהול ושליפת המשאבים הגרפיים.
     */
    PieceRenderer(const Layout& layout, TextureManager& textureManager);

    /**
     * @brief מבצעת פעולת ציור על גבי הקנבס בהתאם לנתוני המצב הנוכחי.
     * @param canvas המשטח הגרפי עליו יתבצע הציור.
     * @param snapshot המידע המכיל את מצב הרכיבים הנוכחי לצורך הצגה.
     */
    void draw(Img& canvas, const GameSnapshot& snapshot);
};