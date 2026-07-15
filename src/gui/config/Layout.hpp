#pragma once

#include "Position.hpp"

/**
 * @class Layout
 * @brief אחראי על כל חישובי המיקום במסך.
 */
class Layout
{
private:

    int rows;

    int cols;

    int cellSize;

    int boardOffsetX;

    int boardOffsetY;

public:

    Layout(int rows, int cols);

    int getCellSize() const;

    int getBoardOffsetX() const;

    int getBoardOffsetY() const;

    int boardToPixelX(int col) const;

    int boardToPixelY(int row) const;

    Position pixelToBoard(int x, int y) const;
};