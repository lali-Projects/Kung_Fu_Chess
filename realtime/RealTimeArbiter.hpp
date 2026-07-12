#pragma once
#include "Board.hpp"
#include "Motion.hpp"
#include <optional>
#include <memory>

class GameEngine; // הצהרה מקדימה בלבד

class RealTimeArbiter {
private:
    Board& board;
    std::optional<Motion> activeMotion;

public:
    RealTimeArbiter(Board& b) : board(b) {}

    void startMotion(std::shared_ptr<Piece> piece, Position src, Position dst, int startTime, int travelTime);
    void advanceTime(int currentTime, GameEngine& engine);
    bool hasActiveMotion() const;
};