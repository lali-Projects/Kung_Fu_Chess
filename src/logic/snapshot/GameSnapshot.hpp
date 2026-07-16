#pragma once

#include <vector>
#include <optional>

#include "PieceSnapshot.hpp"
#include "Position.hpp"


class GameSnapshot
{
private:

    std::vector<PieceSnapshot> pieces;

    int currentTimeMs;

    bool gameOver;

    std::optional<Position> selectedPosition;


public:

    GameSnapshot(
        const std::vector<PieceSnapshot>& pieces,
        int currentTimeMs,
        bool gameOver,
        std::optional<Position> selectedPosition = std::nullopt);


    const std::vector<PieceSnapshot>& getPieces() const;


    int getCurrentTime() const;


    bool isGameOver() const;


    const std::optional<Position>& getSelectedPosition() const;


    void setSelectedPosition(
        const std::optional<Position>& position);
};