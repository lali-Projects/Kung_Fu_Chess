#include "GameController.hpp"
#include <iostream>

GameController::GameController(Board& b, GameEngine& ge)
    : board(b), gameEngine(ge), selectedPosition(std::nullopt) {}

MoveResult GameController::jump(const Position& pos) {
    if (!board.isInsideBoard(pos)) {
        return {false, "outsideBoard"};
    }

    MoveResult result = gameEngine.requestJump(pos);

    if (!result.success) {
        std::cout << "ERROR " << result.reason << std::endl;
    }
    return result;
}

MoveResult GameController::click(const Position& pos) {
    return click(pos, selectedPosition);
}

MoveResult GameController::click(
    const Position& pos,
    std::optional<Position>& selection) {
    std::cout
    << "[CONTROLLER] click "
    << pos.row
    << ","
    << pos.col
    << std::endl;
    if (!board.isInsideBoard(pos)) {
        return {false, "outsideBoard"};
    }

    auto clickedPiece = board.getPieceAt(pos);

    if (!selection.has_value()) {
        if (clickedPiece) {
            selection = pos;
            return {true, "piece_selected"};
        }
        return {false, "empty_square"};
    }

    auto selectedPiece = board.getPieceAt(selection.value());

    if (clickedPiece && selectedPiece && pos == selection.value()) {
        MoveResult result = gameEngine.requestJump(pos);
        selection = std::nullopt;
        return result;
    }

    if (clickedPiece && selectedPiece && clickedPiece->getSide() == selectedPiece->getSide()) {
        selection = pos;
        return {true, "change_selection"};
    }
std::cout
    << "[CONTROLLER] calling requestMove from "
    << selection->row
    << ","
    << selection->col
    << " to "
    << pos.row
    << ","
    << pos.col
    << std::endl;
    // ניסיון תנועה רגילה
    MoveResult result = gameEngine.requestMove(selection.value(), pos);
    selection = std::nullopt;

    return result;
}

void GameController::clearSelection() {
    selectedPosition = std::nullopt;
}

const std::optional<Position>& GameController::getSelectedPosition() const {
    return selectedPosition;
}
