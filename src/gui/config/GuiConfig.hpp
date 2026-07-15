#pragma once

#include <string>

/**
 * @class GuiConfig
 * @brief מרכז את כל ההגדרות הקבועות של שכבת ה-GUI.
 *
 * כל ההגדרות הן Static כך שניתן לגשת אליהן מכל מקום
 * מבלי ליצור מופע של המחלקה.
 */
class GuiConfig
{
public:

    // ------------------------
    // Window
    // ------------------------

    static const std::string WINDOW_TITLE;

    static const int WINDOW_WIDTH;

    static const int WINDOW_HEIGHT;

    static const int FPS;

    // ------------------------
    // Board
    // ------------------------

    static const int BOARD_MARGIN;

    // ------------------------
    // Animation
    // ------------------------

    static const int DEFAULT_ANIMATION_DURATION;
};