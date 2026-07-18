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
 
    int windowWidth;    
    int windowHeight;  
    int boardRows;      
    int boardCols;     
    int boardWidth;     
    int boardHeight;    
    int cellSize;       
    int boardOffsetX;  
    int boardOffsetY;  

public:
    // בנאי המחלקה: מאתחל את המימדים הבסיסיים של הלוח והחלון
    Layout(int windowWidth, int windowHeight, int boardRows, int boardCols);

   
    int getWindowWidth() const;
    int getWindowHeight() const;
    int getBoardWidth() const;
    int getBoardHeight() const;
    int getCellSize() const;
    int getBoardOffsetX() const;
    int getBoardOffsetY() const;

    /**
     * @brief הופך מיקום לוגי (שורה/עמודה) למיקום פיקסל בודד במסך.
     */
    cv::Point boardToPixel(const Position& position) const;

    /**
     * @brief הופך מיקום פיקסלים (למשל לחיצת עכבר) למיקום לוגי (שורה/עמודה).
     */
    Position pixelToBoard(int x, int y) const;

    /**
     * @brief מייצר מלבן (Rect) המייצג את התחום הגיאומטרי של משבצת ספציפית.
     */
    cv::Rect getCellRect(const Position& position) const;

    /**
     * @brief מעדכן את מימדי הלוח ומחשב מחדש גדלים ומרכוזים (למשל בעת שינוי גודל חלון).
     */
    void setBoardSize(int width, int height);
};