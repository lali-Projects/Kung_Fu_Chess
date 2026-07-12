#pragma once
#include "Board.hpp"
#include "RuleEngine.hpp"
#include "GameConfig.hpp"
#include "RealTimeArbiter.hpp"
#include <string>
#include <algorithm>
#include <cmath>
#include <stdexcept>

struct MoveResult {
    bool success;
    std::string reason;
};

class GameEngine {
private:
    Board& board;
    RuleEngine& ruleEngine;
    RealTimeArbiter arbiter; // מנהל התנועות בזמן אמת
    int currentTimeMs;
    bool isGameOver; // דגל המציין אם המשחק הסתיים

    void validateNoActiveMotion() const {
        if (arbiter.hasActiveMotion()) {
            throw std::runtime_error("MOTION_IN_PROGRESS");
        }
    }

    void validateMoveLegality(const Position& from, const Position& to) const {
        MoveValidation validation = ruleEngine.isValidMove(to, from, board);
        if (!validation.is_valid) {
            throw std::runtime_error(validation.reason);
        }
    }

    int calculateTravelTime(const Position& from, const Position& to) const {
        int rowDiff = std::abs(to.getRow() - from.getRow());
        int colDiff = std::abs(to.getCol() - from.getCol());
        int distance = std::max(rowDiff, colDiff);
        return distance * GameConfig::DEFAULT_TRAVEL_TIME_MS;
    }

public:
    GameEngine(Board& b, RuleEngine& re) 
        : board(b), ruleEngine(re), arbiter(b), 
          currentTimeMs(GameConfig::INITIAL_TIME_MS), isGameOver(false) {}

    /**
     * @brief מבקש מהלך. דוחה בקשות אם המשחק הסתיים או שיש תנועה פעילה.
     */
    MoveResult requestMove(const Position& from, const Position& to) {
        try {
            if (isGameOver) throw std::runtime_error("game_over");
            
            validateNoActiveMotion();
            validateMoveLegality(from, to);

            int travelTime = calculateTravelTime(from, to);
            arbiter.startMotion(board.getPieceAt(from), from, to, currentTimeMs, travelTime);
            
            return {true, "ok"};
        } catch (const std::exception& e) {
            return {false, e.what()};
        }
    }

    /**
     * @brief מקדם את הזמן ומעביר את האובייקט הנוכחי לארביטר לבדיקת תנאי סיום.
     */
    void wait(int milliseconds) {
        currentTimeMs += milliseconds;
        arbiter.advanceTime(currentTimeMs, *this);
    }

    /**
     * @brief נקראת על ידי הארביטר כאשר מלך מוכה.
     */
    void signalGameOver() {
        isGameOver = true;
    }

    const Board& getBoard() const { return board; }
    int getCurrentTime() const { return currentTimeMs; }
};