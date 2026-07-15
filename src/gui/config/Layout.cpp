#include "Layout.hpp"

#include <algorithm>

Layout::Layout(
    int windowWidth,
    int windowHeight,
    int boardRows,
    int boardCols)
    :
    windowWidth(windowWidth),
    windowHeight(windowHeight),
    boardRows(boardRows),
    boardCols(boardCols),
    boardWidth(windowWidth),
    boardHeight(windowHeight),
    cellSize(0),
    boardOffsetX(0),
    boardOffsetY(0)
{
    setBoardSize(
        boardWidth,
        boardHeight);
}

int Layout::getWindowWidth() const
{
    return windowWidth;
}

int Layout::getWindowHeight() const
{
    return windowHeight;
}

int Layout::getBoardWidth() const
{
    return boardCols * cellSize;
}

int Layout::getBoardHeight() const
{
    return boardRows * cellSize;
}

int Layout::getCellSize() const
{
    return cellSize;
}

int Layout::getBoardOffsetX() const
{
    return boardOffsetX;
}

int Layout::getBoardOffsetY() const
{
    return boardOffsetY;
}

cv::Point Layout::boardToPixel(
    const Position& position) const
{
    return cv::Point(
        boardOffsetX + position.getCol() * cellSize,
        boardOffsetY + position.getRow() * cellSize);
}

Position Layout::pixelToBoard(
    int x,
    int y) const
{
    int col =
        (x - boardOffsetX) / cellSize;

    int row =
        (y - boardOffsetY) / cellSize;

    return Position(row, col);
}

cv::Rect Layout::getCellRect(
    const Position& position) const
{
    cv::Point topLeft =
        boardToPixel(position);

    return cv::Rect(
        topLeft.x,
        topLeft.y,
        cellSize,
        cellSize);
}
void Layout::setBoardSize(
    int width,
    int height)
{
    boardWidth = width;
    boardHeight = height;

    cellSize = std::min(
        boardWidth / boardCols,
        boardHeight / boardRows);

    boardOffsetX =
        (windowWidth - boardWidth) / 2;

    boardOffsetY =
        (windowHeight - boardHeight) / 2;
}