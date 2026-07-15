#pragma once

#include <string>


/**
 * @class GuiConfig
 * @brief מרכז את כל ההגדרות הקבועות של שכבת ה-GUI.
 *
 * מכיל:
 * - הגדרות חלון
 * - הגדרות לוח
 * - הגדרות אנימציה
 * - נתיבי משאבים
 *
 * המחלקה אינה מכירה:
 * - GameEngine
 * - Board
 * - Piece
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


    // ------------------------
    // Resources
    // ------------------------

    // תיקיית כל המשאבים
    static const std::string ASSETS_PATH;


    // תמונת הלוח
    static const std::string BOARD_TEXTURE_PATH;


    // תיקיית הכלים
    static const std::string PIECES_PATH;
};