#pragma once
#include "Piece.hpp"
#include <memory>
#include <string>

/**
 * @brief מנוע יצירת הכלים (PieceEngine).
 * אחראי על הפיכת נתונים גולמיים לאובייקטים של כלים.
 */
class PieceEngine {
public:
    // יצירת כלי חדש על סמך סוג, צד ומיקום.
    static std::shared_ptr<Piece> createPiece(char typeChar, Side side, Position pos) {
        PieceType type;
        
        switch (typeChar) {
            case 'K': type = PieceType::KING;   break;
            case 'Q': type = PieceType::QUEEN;  break;
            case 'R': type = PieceType::ROOK;   break;
            case 'B': type = PieceType::BISHOP; break;
            case 'N': type = PieceType::KNIGHT; break;
            case 'P': type = PieceType::PAWN;   break;
            default: return nullptr;
        }

        return std::make_shared<Piece>(0, side, type, pos);
    }

    // בדיקה האם סוג הכלי מוכר למנוע
    static bool isKnownType(char type) {
        return (type == 'K' || type == 'Q' || type == 'R' || type == 'B' || type == 'N' || type == 'P');
    }
};