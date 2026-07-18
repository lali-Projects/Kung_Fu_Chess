#pragma once

#include "Img.hpp"
#include <string>
#include <functional>

/**
 * @class Window
 * @brief מנהלת חלון גרפי להצגת תמונות וטיפול באירועי קלט.
 * 
 * מחלקה זו עוטפת את הפונקציונליות של ספריית הגרפיקה ליצירת חלון,
 * הצגת תמונה בתוכו, ורישום אירועי עכבר בצורה נוחה למשתמש.
 */
class Window {
private:
    std::string title;                         
    bool open;                                  
    std::function<void(int, int)> mouseCallback; 

public:
    /**
     * @brief בנאי המחלקה - מאתחל חלון חדש.
     * @param title הכותרת שתוצג בראש החלון.
     */
    Window(const std::string& title);

    /**
     * @brief מציגה את התמונה בחלון ומעבדת אירועי מערכת (מקלדת/עכבר).
     * יש לקרוא לפונקציה זו בתוך הלולאה הראשית של התוכנית.
     * @param image אובייקט ה-Img להצגה.
     */
    void show(const Img& image);

    /**
     * @brief בודקת האם החלון עדיין פעיל ופתוח.
     * @return true אם החלון פתוח, false אם נסגר.
     */
    bool isOpen() const;

    /**
     * @brief מגדירה את הפונקציה שתופעל כאשר המשתמש מבצע לחיצה בחלון.
     * @param callback פונקציה המקבלת קואורדינטות (x, y).
     */
    void setMouseCallback(std::function<void(int, int)> callback);

private:
   
    static void mouseHandler(int event, int x, int y, int flags, void* userdata);
};