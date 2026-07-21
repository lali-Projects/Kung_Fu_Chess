#pragma once

#include <memory>

#include "Board.hpp"
#include "MotionManager.hpp"
#include "JumpManager.hpp"
#include "RestManager.hpp"
#include "MoveExecutor.hpp"

class GameEngine;

/**
 * @class RealTimeArbiter
 *
 * @brief Responsible for all real-time game actions.
 *
 * Responsibilities:
 * - Manage Motion timing
 * - Manage Jump timing
 * - Manage Rest timing
 * - Execute MoveExecutor after motion completion
 *
 * Does NOT:
 * - Check move legality
 * - Manage game rules
 * - Handle promotion
 * - Handle game over
 */
class RealTimeArbiter
{
private:
    Board& board;
    MotionManager motionManager;
    JumpManager jumpManager;
    RestManager restManager;
    MoveExecutor moveExecutor;

    void processMotionCompletion(GameEngine& engine);

public:
    explicit RealTimeArbiter(Board& board);

    //---------------------------------
    // Time
    //---------------------------------
    void advanceTime(int currentTime, GameEngine& engine);

    //---------------------------------
    // Motion
    //---------------------------------
    bool startMotion(std::shared_ptr<Piece> piece, Position from, Position to, int startTime);

    bool hasActiveMotion() const;

    //---------------------------------
    // Jump
    //---------------------------------
    bool startJump(std::shared_ptr<Piece> piece, int startTime, int duration);

    bool hasActiveJump() const;

    //---------------------------------
    // Rest
    //---------------------------------
    bool hasActiveRestFor(std::shared_ptr<Piece> piece) const;

    //---------------------------------
    // Animation
    //---------------------------------
    bool hasActiveAnimation(std::shared_ptr<Piece> piece) const;

    int getAnimationStartTime(std::shared_ptr<Piece> piece) const;

    //---------------------------------
    // Motion Queries
    //---------------------------------
    bool hasActiveMotionFor(std::shared_ptr<Piece> piece) const;

    Position getMotionStart(std::shared_ptr<Piece> piece) const;

    Position getMotionDestination(std::shared_ptr<Piece> piece) const;

    int getMotionStartTime(std::shared_ptr<Piece> piece) const;

    int getMotionFinishTime(std::shared_ptr<Piece> piece) const;
};