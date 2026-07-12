#pragma once
#include "Board.hpp"
#include "Position.hpp"
#include "GameEngine.hpp"
#include "BoardMapper.hpp"
#include <optional>

class GameController {
private:
    Board& board;
    GameEngine& gameEngine;
    std::optional<Position> selectedPosition;

public:
    GameController(Board& b, GameEngine& ge) : board(b), gameEngine(ge) {}

    void click(int pixelX, int pixelY) {
        // 1. תרגום לפיקסלים (פונקציה פשוטה)
        Position pos = BoardMapper::pixelToPosition(pixelX, pixelY);

        if (!board.isInsideBoard(pos)) {
            return;
        }

        std::shared_ptr<Piece> clickedPiece = board.getPieceAt(pos);

        // 3. לוגיקת המצבים
        if (!selectedPosition.has_value()) {
            if (clickedPiece != nullptr) {
                selectedPosition = pos;
            }
        } 
        else {
            std::shared_ptr<Piece> selectedPiece = board.getPieceAt(selectedPosition.value());

            // החלפת בחירה
            if (clickedPiece != nullptr && selectedPiece != nullptr && 
                clickedPiece->getSide() == selectedPiece->getSide()) {
                selectedPosition = pos;
            } 
            // ניסיון מהלך
            else {
                MoveResult result = gameEngine.requestMove(selectedPosition.value(), pos);
                clearSelection();
            }
        }
    }

    void clearSelection() {
        selectedPosition = std::nullopt;
    }
};