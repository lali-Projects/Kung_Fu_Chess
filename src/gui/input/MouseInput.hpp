#pragma once

#include "GameController.hpp"
#include "Layout.hpp"

/**
 * @class MouseInput
 * @brief מחלקה המגשרת בין אירועי קלט של העכבר לבין הלוגיקה של המשחק.
 * 
 * מחלקה זו אחראית לקבל קואורדינטות של לחיצת עכבר (בפיקסלים), להמיר אותן למיקומים 
 * לוגיים על הלוח באמצעות ה-Layout, ולהעביר את הפעולה לביצוע ב-GameController.
 */
class MouseInput
{
private:
    GameController& controller; 
    Layout& layout;            

public:
    /**
     * @brief בנאי המחלקה.
     * @param controller הפניה ל-GameController לביצוע פעולות משחק.
     * @param layout הפניה ל-Layout לתרגום מיקומי מסך ללוח.
     */
    MouseInput(GameController& controller, Layout& layout);

    /**
     * @brief מעבדת אירוע לחיצת עכבר.
     * 
     * פונקציה זו מתרגמת את נקודת הלחיצה (x, y) למיקום בלוח, 
     * ומפעילה את הלוגיקה המתאימה ב-GameController.
     * 
     * @param x קואורדינטת ה-X של הלחיצה.
     * @param y קואורדינטת ה-Y של הלחיצה.
     */
    void click(int x, int y);
};