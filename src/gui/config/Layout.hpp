#pragma once

#include <opencv2/core.hpp>
#include "Position.hpp"

class Layout
{
private:

    int windowWidth;
    int windowHeight;

    int boardRows;
    int boardCols;

    int cellSize;

    int boardOffsetX;
    int boardOffsetY;

public:

    Layout(
        int windowWidth,
        int windowHeight,
        int boardRows,
        int boardCols);

    int getWindowWidth() const;

    int getWindowHeight() const;

    int getBoardWidth() const;

    int getBoardHeight() const;

    int getCellSize() const;

    int getBoardOffsetX() const;

    int getBoardOffsetY() const;

    cv::Point boardToPixel(
        const Position& position) const;

    Position pixelToBoard(
        int x,
        int y) const;

    cv::Rect getCellRect(
        const Position& position) const;
};