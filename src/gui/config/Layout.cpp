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
    boardCols(boardCols)
{
    cellSize =
        std::min(
            windowWidth / boardCols,
            windowHeight / boardRows);

    boardOffsetX =
        (windowWidth - boardCols * cellSize) / 2;

    boardOffsetY =
        (windowHeight - boardRows * cellSize) / 2;
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