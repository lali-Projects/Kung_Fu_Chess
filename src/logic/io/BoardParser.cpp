#include "BoardParser.hpp"
#include "Board.hpp"
#include "GameConfig.hpp"
#include "PieceEngine.hpp"
#include <sstream>
#include <stdexcept>

// -------------------------------------------------------------------------
// מטרת הפונקציה: קריאת השורות ששייכות ללוח מתוך זרם הקלט (קובץ / מקלדת).
// -------------------------------------------------------------------------
std::vector<std::string> BoardParser::readRawLines(std::istream& is) {
    std::vector<std::string> lines;
    std::string line;
    bool readingBoard = false; 

    while (std::getline(is, line)) {
        // התעלמות משורות ריקות או שורות שמכילות רק רווחים/טאבים
        if (line.empty() || line.find_first_not_of(" \t\r\n") == std::string::npos) {
            continue;
        }
        
        // אם נתקלנו בכותרת "Board:", נדליק את הדגל ונתחיל לקרוא בשורות הבאות
        if (line.find("Board:") != std::string::npos) { 
            readingBoard = true; 
            continue; 
        }
        
        // אם הגענו לכותרת "Commands:", סימן שסיימנו לקרוא את הלוח ונעצור מיד
        if (line.find("Commands:") != std::string::npos) { 
            break; 
        }
        
        // אם הדגל דלוק, השורה הנוכחית היא חלק מהלוח - נשמור אותה
        if (readingBoard) {
            lines.push_back(line);
        }
    }
    return lines;
}

// -------------------------------------------------------------------------
// מטרת הפונקציה: ספירת המילים (טוקנים) בשורה הראשונה כדי לקבוע את כמות העמודות.
// -------------------------------------------------------------------------
int BoardParser::calculateColumns(const std::string& firstLine) {
    std::stringstream ssFirst(firstLine);
    std::string temp;
    int cols = 0;
    
    // ספירת כמות הכלים/משבצות בשורה באמצעות הזרמת מילים
    while (ssFirst >> temp) {
        cols++;
    }
    return cols;
}

// -------------------------------------------------------------------------
// מטרת הפונקציה: פירוק שורה אחת לטוקנים, אימותם, יצירת הכלים והצבתם בלוח.
// -------------------------------------------------------------------------
void BoardParser::parseSingleRow(Board& board, const std::string& line, int rowIndex, int expectedCols) {
    std::stringstream ss(line);
    std::string token;

    for (int c = 0; c < expectedCols; ++c) {

        if (!(ss >> token)) {
            throw std::runtime_error("ROW_WIDTH_MISMATCH");
        }

        if (token.length() == 1 && token[0] == GameConfig::EMPTY_CELL) {
            continue;
        }

        auto piece = PieceEngine::createFromToken(token, Position(rowIndex, c));
        
        // אם המנוע החזיר nullptr, סימן שהטוקן לא חוקי (צד שגוי או סוג כלי לא מוכר)
        if (!piece) {
            throw std::runtime_error("UNKNOWN_TOKEN");
        }
        
        // הזנת הכלי אל תוך הלוח במיקום המתאים
        board.setPieceAt(rowIndex, c, piece);
    }
}

// -------------------------------------------------------------------------
// מטרת הפונקציה: ה-Main פונקציה של ה-Parser. מנווטת את כל תהליך יצירת הלוח מההתחלה ועד הסוף.
// -------------------------------------------------------------------------
std::unique_ptr<Board> BoardParser::parse(std::istream& is) {
    // 1. נשלוף רק את השורות ששייכות ללוח
    auto lines = readRawLines(is);
    if (lines.empty()) return nullptr; // אם אין לוח, נחזיר מצביע ריק

    // 2. נחשב את כמות העמודות לפי השורה הראשונה
    int cols = calculateColumns(lines[0]);

    // 3. נבנה את אובייקט הלוח (מספר השורות נקבע לפי כמות השורות שקראנו מהקובץ)
    auto board = std::make_unique<Board>((int)lines.size(), cols);

    // 4. נעבור שורה אחר שורה ונפענח את התוכן שלה ישירות לתוך הלוח
    for (int r = 0; r < (int)lines.size(); ++r) {
        parseSingleRow(*board, lines[r], r, cols);
    }

    return board;
}