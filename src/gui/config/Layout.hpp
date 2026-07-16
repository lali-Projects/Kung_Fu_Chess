#pragma once

#include <opencv2/core.hpp>
#include "Position.hpp"

/**
 * @brief המחלקה אחראית על ניהול הגיאומטריה של המשחק.
 * היא משמשת כשכבת התרגום בין עולם הנתונים הלוגי (Rows/Cols)
 * לבין עולם התצוגה הפיזי (Pixels), כדי להבטיח שהלוגיקה תישאר
 * אדישה לחלוטין להגדרות התצוגה.
 */
class Layout
{
private:
    // הערכים הללו נשמרים כאן כדי להבטיח עקביות בכל חישובי הקואורדינטות.
    int windowWidth;
    int windowHeight;
    int boardRows;
    int boardCols;
    int boardWidth;
    int boardHeight;
    int cellSize;     // נקבע דינמית כדי למנוע עיוות של המשבצות.
    int boardOffsetX; // נועד למרכז את הלוח בחלון בצורה אוטומטית.
    int boardOffsetY;

public:
    Layout(int windowWidth, int windowHeight, int boardRows, int boardCols);

    // Getters: מספקים נתונים גיאומטריים ל-Renderers מבלי לחשוף את הלוגיקה הפנימית.
    int getWindowWidth() const;
    int getWindowHeight() const;
    int getBoardWidth() const;
    int getBoardHeight() const;
    int getCellSize() const;
    int getBoardOffsetX() const;
    int getBoardOffsetY() const;

    /**
     * @brief הופך לוגיקה למיקום מסך.
     * הכרחי כדי ששכבת ה-Renderer לא תצטרך לבצע חישובים מורכבים.
     */
    cv::Point boardToPixel(const Position& position) const;

    /**
     * @brief הופך קלט עכבר (Pixels) ללוגיקה (Board Position).
     * מאפשר ל-InputController לעבוד עם הלוח ללא צורך בהבנה של גודל החלון.
     */
    Position pixelToBoard(int x, int y) const;

    /**
     * @brief מייצר ריבוע תצוגה עבור כלי.
     * משמש את ה-PieceRenderer לצורך ציור או סימון (Highlight).
     */
    cv::Rect getCellRect(const Position& position) const;

    /**
     * @brief עדכון גיאומטריה.
     * מאפשר ל-Layout להתאים את עצמו דינמית אם גודל החלון משתנה
     * מבלי לאתחל מחדש את כל המשחק.
     */
    void setBoardSize(int width, int height);
};