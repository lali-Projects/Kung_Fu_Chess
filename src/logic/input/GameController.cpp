#include "GameController.hpp"
#include <iostream> 

GameController::GameController(Board& b, GameEngine& ge) 
    : board(b), gameEngine(ge), selectedPosition(std::nullopt) {}

MoveResult GameController::jump(const Position& pos) {
    if (!board.isInsideBoard(pos)) {
         return {false, "outsideBoard"};
    }
    
    // קריאה ל-GameEngine לבצע את הלוגיקה
    MoveResult result = gameEngine.requestJump(pos);
    
    if (!result.success) {
        std::cout << "ERROR " << result.reason << std::endl;
    }
    return result;
}

MoveResult GameController::click(const Position& pos) {
    // 1. תרגום לפיקסלים (פונקציה פשוטה)

    if (!board.isInsideBoard(pos)) {
        return {false, "outsideBoard"};
    }

    std::shared_ptr<Piece> clickedPiece = board.getPieceAt(pos);
    // 3. לוגיקת המצבים
    if (!selectedPosition.has_value()) {
        if (clickedPiece != nullptr) {
            selectedPosition = pos;
             return {true, "piece_selected"};
        }
         return {false, "empty_square"};
    } 
    else {
        std::shared_ptr<Piece> selectedPiece = board.getPieceAt(selectedPosition.value());

        // החלפת בחירה
        if (clickedPiece != nullptr && selectedPiece != nullptr && 
            clickedPiece->getSide() == selectedPiece->getSide()) {
            selectedPosition = pos;
            return  {true, "change_selection"};;
        } 
        // ניסיון מהלך
        else {
            MoveResult result = gameEngine.requestMove(selectedPosition.value(), pos);
            clearSelection();
            return result;
        }
    }
     return {false, "no"};
}

void GameController::clearSelection() {
    selectedPosition = std::nullopt;
}

 const std::optional<Position>& GameController::getSelectedPosition() const {
    return selectedPosition;
}
