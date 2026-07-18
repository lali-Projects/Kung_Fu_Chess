#include "Layout.hpp"
#include <algorithm>

Layout::Layout(int windowWidth, int windowHeight, int boardRows, int boardCols)
    : windowWidth(windowWidth),
      windowHeight(windowHeight),
      boardRows(boardRows),
      boardCols(boardCols),
      boardWidth(windowWidth),
      boardHeight(windowHeight),
      cellSize(0),
      boardOffsetX(0),
      boardOffsetY(0)
{
    setBoardSize(boardWidth, boardHeight);
}


int Layout::getWindowWidth() const { return windowWidth; }
int Layout::getWindowHeight() const { return windowHeight; }

int Layout::getBoardWidth() const 
{ 
    return boardCols * cellSize; 
}

int Layout::getBoardHeight() const 
{ 
    return boardRows * cellSize; 
}

int Layout::getCellSize() const { return cellSize; }
int Layout::getBoardOffsetX() const { return boardOffsetX; }
int Layout::getBoardOffsetY() const { return boardOffsetY; }

cv::Point Layout::boardToPixel(const Position& position) const
{
    // המרה ממרחב לוגי למרחב פיקסלים:
    // אנחנו מוסיפים את ה-Offset כדי להתחשב במיקום הלוח במסך,
    // ומכפילים את המיקום הלוגי ב-cellSize כדי למצוא את הפינה העליונה של המשבצת.
    // זה מאפשר ל-Renderer לצייר ללא ידיעה על הלוגיקה הפנימית של הלוח.
    return cv::Point(
        boardOffsetX + position.getCol() * cellSize,
        boardOffsetY + position.getRow() * cellSize);
}

Position Layout::pixelToBoard(int x, int y) const
{
    // הפעולה ההפוכה מהמרת פיקסלים ללוגיקה (נדרש לקלט עכבר):
    // 1. חיסור ה-Offset "מנקה" את שולי הלוח ומביא אותנו לנקודת ההתחלה (0,0).
    // 2. חילוק ב-cellSize הופך רצף פיקסלים רציף למדד בדיד (אינדקס תא).
    // השלמים (int) מבצעים כאן Floor באופן טבעי, מה שמתאים בדיוק.
    int col = (x - boardOffsetX) / cellSize;
    int row = (y - boardOffsetY) / cellSize;

    return Position(row, col);
}

cv::Rect Layout::getCellRect(const Position& position) const
{
    cv::Point topLeft = boardToPixel(position);
    return cv::Rect(topLeft.x, topLeft.y, cellSize, cellSize);
}

void Layout::setBoardSize(int width, int height)
{
    boardWidth = width;
    boardHeight = height;
    cellSize = std::min(boardWidth / boardCols, boardHeight / boardRows);
    boardOffsetX = (windowWidth - boardWidth) / 2;
    boardOffsetY = (windowHeight - boardHeight) / 2;
}