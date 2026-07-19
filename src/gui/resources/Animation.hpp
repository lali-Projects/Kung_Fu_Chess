#pragma once

#include <string>

#include "Sprite.hpp"

/**
 * @class Animation
 * @brief מייצגת אנימציה אחת של כלי.
 *
 * המחלקה מחזיקה את כל הנתונים הדרושים
 * לניגון אנימציה.
 *
 * אחריות:
 * - החזקת Sprite.
 * - החזקת פרמטרי האנימציה.
 *
 * אינה אחראית לטעינת קבצים.
 * אינה אחראית לציור.
 */
class Animation
{
private:

    Sprite sprite;

    int framesPerSecond;

    bool loop;

    std::string nextState;

    double speedMetersPerSecond;

public:

    Animation();

    //---------------------------------
    // Sprite
    //---------------------------------

    void setSprite(const Sprite& sprite);

    const Sprite& getSprite() const;

    //---------------------------------
    // FPS
    //---------------------------------

    void setFramesPerSecond(int fps);

    int getFramesPerSecond() const;

    //---------------------------------
    // Loop
    //---------------------------------

    void setLoop(bool loop);

    bool isLoop() const;

    //---------------------------------
    // Next state
    //---------------------------------

    void setNextState(const std::string& state);

    const std::string& getNextState() const;

    //---------------------------------
    // Speed
    //---------------------------------

    void setSpeed(double speed);

    double getSpeed() const;

    //---------------------------------
    // Convenience
    //---------------------------------

    bool empty() const;
};