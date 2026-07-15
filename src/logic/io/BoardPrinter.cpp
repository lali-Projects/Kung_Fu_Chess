#include "BoardPrinter.hpp"

void BoardPrinter::print(const Board& board, std::ostream& os) {
    for (int r = 0; r < board.getRows(); ++r) {
        for (int c = 0; c < board.getCols(); ++c) {
            // יצירת אובייקט Position לצורך בדיקת המיקום
            Position currentPos(r, c);
            auto piece = board.getPieceAt(currentPos);
            
            if (!piece) {
                os << ".";
            } else {
                // שימוש במתודה הקיימת ב-Piece כדי לקבל ייצוג טקסטואלי (כמו 'wK')
                os << piece->toCanonicalString();
            }
            
            // הוספת רווח בין עמודות, לא כולל האחרונה
            if (c < board.getCols() - 1) {
                os << " ";
            }
        }
        // ירידת שורה בסוף כל שורת לוח
        os << "\n";
    }
}