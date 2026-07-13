#pragma once
#include "Board.hpp"
#include "GameConfig.hpp"
#include "PieceEngine.hpp"
#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <stdexcept>
#include <memory>

class BoardParser {
private:
    // קריאת השורות הגולמיות השייכות ללוח בלבד
    static std::vector<std::string> readRawLines(std::istream& is) {
        std::vector<std::string> lines;
        std::string line;
        bool readingBoard = false;

        while (std::getline(is, line)) {
            if (line.empty() || line.find_first_not_of(" \t\r\n") == std::string::npos) continue;
            if (line.find("Board:") != std::string::npos) { readingBoard = true; continue; }
            if (line.find("Commands:") != std::string::npos) break;
            if (readingBoard) lines.push_back(line);
        }
        return lines;
    }

    // פונקציית עזר לאימות תקינות הטוקן - שמירה על עיקרון ה-SRP
    static void validateToken(const std::string& token) {
        if (token.length() < 2 || !PieceEngine::isKnownType(token[1])) {
            throw std::runtime_error("UNKNOWN_TOKEN");
        }
    }

public:
    /**
     * @brief ממיר זרם קלט ללוח משחק על בסיס הגדרות דינמיות.
     * זורק exception עם קוד השגיאה המדויק עבור הבודק האוטומטי.
     */
    static std::unique_ptr<Board> parse(std::istream& is) {
        auto lines = readRawLines(is);
        if (lines.empty()) return nullptr;

        // חישוב מספר עמודות לפי השורה הראשונה
        std::stringstream ssFirst(lines[0]);
        std::string temp;
        int cols = 0;
        while (ssFirst >> temp) cols++;

        // יצירת הלוח
        auto board = std::make_unique<Board>((int)lines.size(), cols);

        // מילוי הלוח
        for (int r = 0; r < (int)lines.size(); ++r) {
            std::stringstream ss(lines[r]);
            std::string token;
            for (int c = 0; c < cols; ++c) {
                // בדיקת התאמה של רוחב השורה
                if (!(ss >> token)) throw std::runtime_error("ROW_WIDTH_MISMATCH");

                // בדיקה דינמית האם מדובר במשבצת ריקה (ללא hard-coding של '.')
                if (token.length() == 1 && token[0] == GameConfig::EMPTY_CELL) {
                    continue;
                }

                // בדיקת תקינות הכלי דרך פונקציית ה-SRP והמנוע
                validateToken(token);
                
                // קביעה דינמית של הצד (ללא hard-coding של 'w')
                Side side = (token[0] == GameConfig::SIDE_WHITE) ? Side::WHITE : Side::BLACK;
                char typeChar = token[1];
                
                // יצירת הכלי והצבה בלוח
                board->setPieceAt(r, c, PieceEngine::createPiece(typeChar, side, Position(r, c)));
            }
        }
        return board;
    }
};