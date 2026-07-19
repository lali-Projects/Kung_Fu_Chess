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

MoveResult GameController::click(const Position& pos)
{
    if(!board.isInsideBoard(pos))
        return {false,"outsideBoard"};


    auto clickedPiece =
        board.getPieceAt(pos);



    // אין בחירה קיימת
    if(!selectedPosition.has_value())
    {
        if(clickedPiece)
        {
            selectedPosition = pos;

            return {
                true,
                "piece_selected"
            };
        }

        return {
            false,
            "empty_square"
        };
    }



    auto selectedPiece =
        board.getPieceAt(
            selectedPosition.value());



    // לחיצה כפולה על אותו כלי
    if(clickedPiece &&
       selectedPiece &&
       pos == selectedPosition.value())
    {
        MoveResult result =
            gameEngine.requestJump(pos);

        clearSelection();

        return result;
    }



    // בחירה מחדש של כלי מאותו צד
    if(clickedPiece &&
       selectedPiece &&
       clickedPiece->getSide()
       ==
       selectedPiece->getSide())
    {
        selectedPosition = pos;

        return {
            true,
            "change_selection"
        };
    }



    // ניסיון תנועה רגילה
    MoveResult result =
        gameEngine.requestMove(
            selectedPosition.value(),
            pos);


    clearSelection();

    return result;
}

void GameController::clearSelection() {
    selectedPosition = std::nullopt;
}

 const std::optional<Position>& GameController::getSelectedPosition() const {
    return selectedPosition;
}
