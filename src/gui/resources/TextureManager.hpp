#pragma once

#include <unordered_map>
#include <string>
#include "Img.hpp"

/**
 * @class TextureManager
 * @brief מנהל מחסן נתונים של תמונות (Resource Cache).
 * 
 * מחלקה זו אחראית על:
 * - קריאת קבצי תמונה ממערכת הקבצים.
 * - שמירת התמונות בזיכרון (Caching) למניעת קריאות חוזרות לדיסק.
 * - מתן גישה לאובייקטי תמונה לפי מפתח ייחודי.
 */
class TextureManager
{
private:
    // מיפוי בין מפתח טקסטואלי לאובייקט תמונה בזיכרון
    std::unordered_map<std::string, Img> textures;

public:
    TextureManager() = default;

    /**
     * @brief טוען תמונה ושומר אותה תחת המפתח שסופק.
     */
    void loadTexture(const std::string& key, const std::string& path);

    /**
     * @brief טוען תמונה עם הגדרת מידות (Width, Height) ושומר אותה.
     */
    void loadTexture(const std::string& key, const std::string& path, int width, int height);

    /**
     * @brief מחזיר הפניה לתמונה שנמצאת בזיכרון. 
     * זורק שגיאה אם המפתח לא נמצא.
     */
    Img& getTexture(const std::string& key);

    /**
     * @brief בודק האם המפתח קיים בתוך מאגר הנתונים.
     */
    bool contains(const std::string& key) const;

    /**
     * @brief טעינה ספציפית למשאב תצוגה ראשי.
     */
    void loadBoardTexture(const std::string& path);

    /**
     * @brief טעינה מרוכזת של אוסף משאבים לפי גודל קבוע.
     */
    void loadAllPieceTextures(int cellSize);
};