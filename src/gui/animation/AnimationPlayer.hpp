#pragma once

#include "Animation.hpp"
#include "Img.hpp"

/**
 * @class AnimationPlayer
 * @brief מחשב איזה Frame יש להציג עבור אנימציה בזמן נתון.
 *
 * אחריות:
 * - חישוב מספר הפריים.
 * - תמיכה באנימציות מחזוריות (Loop).
 * - תמיכה באנימציות חד-פעמיות.
 *
 * אינו אחראי:
 * - טעינת קבצים.
 * - ציור.
 * - שינוי מצב המשחק.
 */
class AnimationPlayer
{
public:

    /**
     * @brief מחזיר את התמונה שיש להציג.
     *
     * @param animation האנימציה.
     * @param animationStartTime זמן תחילת האנימציה.
     * @param currentTime הזמן הנוכחי במשחק.
     */
    const Img& getCurrentFrame(
        const Animation& animation,
        int animationStartTime,
        int currentTime) const;
    std::size_t getCurrentFrameIndex(
        const Animation& animation,
        int animationStartTime,
        int currentTime) const;
private:

    /**
     * @brief מחשב את אינדקס הפריים.
     */
    std::size_t calculateFrameIndex(
        const Animation& animation,
        int animationStartTime,
        int currentTime) const;
};