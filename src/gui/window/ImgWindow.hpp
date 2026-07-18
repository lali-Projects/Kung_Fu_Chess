#pragma once

#include <string>
#include "Img.hpp"

/**
 * @class ImgWindow
 * @brief מחלקה לניהול והצגת חלון גרפי.
 * 
 * מחלקה זו אחראית על שכבת ה-GUI, ומספקת ממשק פשוט ליצירת חלון,
 * הצגת תמונות בתוכו וניהול האירועים הבסיסיים של החלון.
 */
class ImgWindow
{
private:
    std::string windowName; // שם החלון כפי שהוא מופיע במערכת

public:
    /**
     * @brief בנאי המחלקה - מאתחל חלון חדש.
     * @param name שם החלון (ברירת המחדל היא "Kung Fu Chess").
     */
    explicit ImgWindow(const std::string& name = "Kung Fu Chess");

    /**
     * @brief מציגה תמונה בתוך החלון.
     * @param image אובייקט תמונה להצגה (מחלקה מסוג Img).
     */
    void display(const Img& image);

    /**
     * @brief מעבדת אירועי חלון (GUI Events).
     * @details פונקציה זו מאפשרת ל-OpenCV לעבד אירועים (כמו רענון החלון)
     *          ומחכה זמן קצר בהתאם לפרמטר.
     * @param delayMs זמן המתנה במילי-שניות (ברירת מחדל: 1ms).
     */
    void processEvents(int delayMs = 1);

    /**
     * @brief בודקת האם החלון עדיין פתוח ופעיל.
     * @return true אם החלון פתוח, false אם המשתמש סגר אותו.
     */
    bool isOpen() const;

    /**
     * @brief סוגרת את החלון ומשחררת את המשאבים המוקצים לו.
     */
    void close();
};