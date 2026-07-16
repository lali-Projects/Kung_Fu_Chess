#pragma once

#include <vector>
#include <cstddef>

#include "Img.hpp"


/**
 * @class Sprite
 * @brief מייצג רצף תמונות המשמש כאנימציה.
 *
 * Sprite מכיל Frames מסוג Img.
 * כל Frame הוא תמונה אחת באנימציה.
 *
 * המחלקה אינה אחראית לטעינת התמונות.
 */
class Sprite
{
private:

    std::vector<Img> frames;


public:

    /**
     * @brief מוסיף Frame חדש ל־Sprite.
     *
     * @param frame תמונה להוספה.
     */
    void addFrame( const Img& frame );


    /**
     * @brief מחזיר Frame לפי אינדקס.
     *
     * אם האינדקס גדול ממספר הפריימים,
     * מתבצעת לולאה.
     *
     * @param index מספר הפריים.
     */
    const Img& getFrame(  std::size_t index) const;


    /**
     * @brief מחזיר מספר Frames קיימים.
     */
    std::size_t getFrameCount() const;


    /**
     * @brief בדיקה האם Sprite ריק.
     */
    bool empty() const;


    /**
     * @brief מחיקת כל הפריימים.
     */
    void clear();
};