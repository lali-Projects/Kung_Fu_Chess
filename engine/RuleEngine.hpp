#pragma once
#include <map>
#include <memory>
#include <string>
#include "Board.hpp"
#include "IMoveRule.hpp"
#include "RuleKing.hpp"
#include "RuleQueen.hpp"
#include "RuleRook.hpp"
#include "RuleBishop.hpp"
#include "RuleKnight.hpp"
#include "RulePawn.hpp"

struct MoveValidation {
    bool is_valid;
    std::string reason;
};

class RuleEngine {
private:
    // שימוש במפה לניהול אסטרטגיות תנועה
    std::map<PieceType, std::unique_ptr<IMoveRule>> moveRules;

public:
    RuleEngine() {
        moveRules[PieceType::KING]   = std::make_unique<RuleKing>();
        moveRules[PieceType::QUEEN]  = std::make_unique<RuleQueen>();
        moveRules[PieceType::ROOK]   = std::make_unique<RuleRook>();
        moveRules[PieceType::BISHOP] = std::make_unique<RuleBishop>();
        moveRules[PieceType::KNIGHT] = std::make_unique<RuleKnight>();
        moveRules[PieceType::PAWN]   = std::make_unique<RulePawn>();
    }

    MoveValidation isValidMove(const Position& to, const Position& from, const Board& grid) {
        // 1. בדיקת גבולות הלוח
        if (!grid.isInsideBoard(to)) {
            return {false, "outside_board"};
        }

        // 2. בדיקת קיום כלי במקור
        auto sourcePiece = grid.getPieceAt(from);
        if (!sourcePiece) {
            return {false, "empty_source"};
        }

        // 3. בדיקת "אש ידידותית"
        auto destinationPiece = grid.getPieceAt(to);
        if (destinationPiece && sourcePiece->getSide() == destinationPiece->getSide()) {
            return {false, "friendly_destination"};
        }

        // 4. בדיקת חוקיות לפי סוג הכלי
        PieceType type = sourcePiece->getType();
        auto legalMoves = moveRules[type]->getLegalDestinations(grid, *sourcePiece);

        if (legalMoves.find(to) == legalMoves.end()) {
            return {false, "not_valid_path_for_this_type"};
        }

        return {true, "ok"};
    }
};