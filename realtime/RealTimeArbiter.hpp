#pragma once
#include "Board.hpp"
#include "Motion.hpp"
#include <optional>
#include <memory>

class GameEngine; // הצהרה מקדימה

class RealTimeArbiter {
private:
    Board& board;
    std::optional<Motion> activeMotion;

    // פונקציות עזר פרטיות לניהול תהליך התנועה
    void processMotionCompletion(GameEngine& engine);
    void handlePawnPromotion(std::shared_ptr<Piece> piece, const Position& pos);

public:
    RealTimeArbiter(Board& b) : board(b) {}

    void startMotion(std::shared_ptr<Piece> piece, Position src, Position dst, int startTime, int travelTime);
    
    // בודקת האם הגיע זמן ביצוע התנועה ועדכון הלוח
    void advanceTime(int currentTime, GameEngine& engine);

    bool hasActiveMotion() const;
};