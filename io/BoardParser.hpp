#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <memory>

// Forward Declaration - מודיע לקומפילר שקיים אובייקט בשם Board בלי לטעון את כל הקובץ שלו כאן
class Board; 

/**
 * @class BoardParser
 * @brief מחלקת שירות (Utility Class) האחראית על קריאת קובץ הקלט וטעינת הלוח.
 */
class BoardParser {
private:
    /**
     * @brief מטרת הפונקציה: סינון וקריאת השורות הגולמיות השייכות ללוח בלבד.
     * הפונקציה מתעלמת משורות ריקות, ומבודדת רק את הטקסט שנמצא בין התגית "Board:" לתגית "Commands:".
     */
    static std::vector<std::string> readRawLines(std::istream& is);

    /**
     * @brief מטרת הפונקציה: חישוב מספר העמודות הנדרש ללוח.
     * הפונקציה לוקחת את השורה הראשונה שנקראה, סופרת כמה מילים (טוקנים) יש בה, ולפי זה קובעת את רוחב הלוח.
     */
    static int calculateColumns(const std::string& firstLine);

    /**
     * @brief מטרת הפונקציה: פיענוח שורה בודדת של טקסט והזנת הכלים שבה אל הלוח.
     * הפונקציה עוברת משבצת-משבצת בשורה, בודקת התאמה של רוחב השורה, מזהה משבצות ריקות, 
     * ומבקשת ממנוע הכלים (PieceEngine) לייצר את הכלים המתאימים עם מזהה ייחודי.
     */
    static void parseSingleRow(Board& board, const std::string& line, int rowIndex, int expectedCols);

public:
    /**
     * @brief מטרת הפונקציה: הפונקציה הראשית והציבורית שמנהלת את כל תהליך בניית הלוח.
     * היא מתאמת בין פונקציות העזר הפנימיות: קוראת את השורות, מחשבת ממדים, מייצרת את אובייקט הלוח,
     * מפעילה את הפיענוח לכל שורה, ומחזירה מצביע ייחודי (unique_ptr) ללוח המוכן.
     * אם מתגלה שגיאה (רוחב לא תואם או כלי לא מוכר), היא תזרוק שגיאה תואמת לבודק האוטומטי.
     */
    static std::unique_ptr<Board> parse(std::istream& is);
};