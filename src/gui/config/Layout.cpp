#include "Layout.hpp"
#include "GuiConfig.hpp"

Layout::Layout(int r, int c)
    : rows(r),
      cols(c)
{
    boardOffsetX = GuiConfig::BOARD_MARGIN;

    boardOffsetY = GuiConfig::BOARD_MARGIN;

    cellSize =
        std::min(
            (GuiConfig::WINDOW_WIDTH - boardOffsetX * 2) / cols,
            (GuiConfig::WINDOW_HEIGHT - boardOffsetY * 2) / rows
        );
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

int Layout::boardToPixelX(int col) const
{
    return boardOffsetX + col * cellSize;
}

int Layout::boardToPixelY(int row) const
{
    return boardOffsetY + row * cellSize;
}

Position Layout::pixelToBoard(int x, int y) const
{
    int col =
        (x - boardOffsetX) / cellSize;

    int row =
        (y - boardOffsetY) / cellSize;

    return Position(row, col);
}