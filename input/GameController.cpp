#include "GameController.hpp"
#include <iostream> // נדרש עבור std::cout ו-std::endl

// מימוש הבנאי בעזרת Initializer List
GameController::GameController(Board& b, GameEngine& ge) 
    : board(b), gameEngine(ge), selectedPosition(std::nullopt) {}

void GameController::jump(int pixelX, int pixelY) {
    Position pos = BoardMapper::pixelToPosition(pixelX, pixelY);
    if (!board.isInsideBoard(pos)) {
        return;
    }
    
    // קריאה ל-GameEngine לבצע את הלוגיקה
    MoveResult result = gameEngine.requestJump(pos);
    
    if (!result.success) {
        std::cout << "ERROR " << result.reason << std::endl;
    }
}

void GameController::click(int pixelX, int pixelY) {
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

void GameController::clearSelection() {
    selectedPosition = std::nullopt;
}