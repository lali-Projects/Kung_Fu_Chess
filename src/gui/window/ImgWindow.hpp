#pragma once

#include <string>
#include "Img.hpp"


/**
 * @class ImgWindow
 * @brief עטיפה של חלון התצוגה.
 *
 * המחלקה היחידה בשכבת GUI
 * שמנהלת את החלון עצמו.
 */
class ImgWindow
{
private:

    std::string windowName;


public:

    /**
     * @brief יצירת חלון חדש.
     */
    explicit ImgWindow(
        const std::string& name = "Kung Fu Chess"
    );


    /**
     * @brief הצגת תמונה בחלון.
     */
    void display(
        const Img& image
    );


    /**
     * @brief טיפול באירועי חלון.
     *
     * מחזיר זמן קצר ל־OpenCV
     * כדי לעבד אירועים.
     */
    void processEvents(
        int delayMs = 1
    );


    /**
     * @brief בדיקה האם החלון עדיין פתוח.
     */
    bool isOpen() const;


    /**
     * @brief סגירת החלון.
     */
    void close();
};