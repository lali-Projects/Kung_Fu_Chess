#pragma once
#include "Board.hpp"
#include "Motion.hpp"
#include "Jump.hpp"
#include <optional>
#include <memory>

class GameEngine;

class RealTimeArbiter {
private:
    Board& board;
    std::optional<Motion> activeMotion;
    std::optional<Jump> activeJump;

    void processMotionCompletion(GameEngine& engine);
    void handlePawnPromotion(std::shared_ptr<Piece> piece, const Position& pos);
    void processJumpCompletion();

public:
    RealTimeArbiter(Board& b) : board(b) {}

    // חתימה מעודכנת: הארביטר דואג לחישוב הזמן הפנימי
    void startMotion(std::shared_ptr<Piece> piece, Position src, Position dst, int startTime);    
    void advanceTime(int currentTime, GameEngine& engine);
    bool hasActiveMotion() const;
    void startJump(std::shared_ptr<Piece> piece, int startTime, int jumpDuration);
    void handleMotionLogic(int currentTime, GameEngine& engine);
    void handleJumpLogic(int currentTime);
    void resolveMotion(GameEngine& engine);
    bool isCollisionWithJump(const Position& pos) const;
    void executeStandardMove(GameEngine& engine);
    void handleJumpCollision(std::shared_ptr<Piece> movingPiece);
};