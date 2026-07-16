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
    // הלוח הוא נגזרת של מספר העמודות וגודל המשבצת. 
    return boardCols * cellSize; 
}

int Layout::getBoardHeight() const 
{ 
    // בדומה לרוחב, הגובה מחושב דינמית כדי למנוע סנכרון לוקה בחסר.
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
    // יצירת מלבן תואם למיקום. השימוש ב-Rect של OpenCV מאפשר לנו
    // להשתמש בפונקציות מובנות של הספרייה (כמו בדיקת התנגשות או ציור מהיר)
    // על ידי ייצוג המשבצת כאובייקט גיאומטרי אחד.
    cv::Point topLeft = boardToPixel(position);
    return cv::Rect(topLeft.x, topLeft.y, cellSize, cellSize);
}

void Layout::setBoardSize(int width, int height)
{
    boardWidth = width;
    boardHeight = height;

    
    // כדי לשמור על היחס (Aspect Ratio) של הלוח כריבוע מושלם.
    // אם נתבסס רק על הרוחב או הגובה, שינוי גודל החלון יעוות את הלוח.
    // אנחנו בוחרים את המגבלה הקטנה ביותר כדי שהלוח תמיד ייכנס במלואו למסך.
    cellSize = std::min(boardWidth / boardCols, boardHeight / boardRows);

   
    // כדי להשיג מרכוז מושלם (Centering). אנחנו מחסירים את גודל הלוח
    // מגודל החלון ומחלקים ב-2. התוצאה היא "שארית" שמתחלקת שווה בשווה 
    // בין הצדדים, מה שיוצר שוליים מאוזנים אסתטית.
    boardOffsetX = (windowWidth - boardWidth) / 2;
    boardOffsetY = (windowHeight - boardHeight) / 2;
}