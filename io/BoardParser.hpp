#pragma once
#include "Board.hpp"
#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <stdexcept>
#include "PieceEngine.hpp"

class BoardParser {
private:
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

public:
    /**
     * @brief ממיר זרם קלט ללוח משחק.
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
                // זריקת שגיאה ללא הקידומת ERROR - ה-main יוסיף אותה
                if (!(ss >> token)) throw std::runtime_error("ROW_WIDTH_MISMATCH");

                if (token != ".") {
                    // בדיקת תקינות דרך המנוע
                    if (!PieceEngine::isKnownType(token[1])) {
                        throw std::runtime_error("UNKNOWN_TOKEN");
                    }
                    
                    Side side = (token[0] == 'w') ? Side::WHITE : Side::BLACK;
                    char typeChar = token[1];
                    
                    // יצירת הכלי והצבה בלוח
                    board->setPieceAt(r, c, PieceEngine::createPiece(typeChar, side, Position(r, c)));
                }
            }
        }
        return board;
    }
};