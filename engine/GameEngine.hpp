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
    /// הלוח של המשחק
    Board& board;

    /// מנוע חוקי השחמט
    RuleEngine& ruleEngine;

    /// אחראי על כל הפעולות המתרחשות לאורך זמן
    RealTimeArbiter& realTimeArbiter;

    /// הזמן הלוגי הנוכחי של המשחק
    int currentTimeMs;

    /// האם המשחק הסתיים
    bool isGameOver;
    void validateNoActiveMotion() const {
        if (realTimeArbiter.hasActiveMotion()) {
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
   GameEngine(Board& board,
           RuleEngine& ruleEngine,
           RealTimeArbiter& arbiter)
    :
      board(board),
      ruleEngine(ruleEngine),
      realTimeArbiter(arbiter),
      currentTimeMs(GameConfig::INITIAL_TIME_MS),
      isGameOver(false)
{}

    const Board& getBoard() const { return board; }
    int getCurrentTime() const { return currentTimeMs; }
    
    /**
     * @brief מבקש מהלך. דוחה בקשות אם המשחק הסתיים או שיש תנועה פעילה.
     */
    MoveResult requestMove(const Position& from, const Position& to) {
        try {
            if (isGameOver) throw std::runtime_error("game_over");
            
            validateNoActiveMotion();
            validateMoveLegality(from, to);

            int travelTime = calculateTravelTime(from, to);
            realTimeArbiter.startMotion(board.getPieceAt(from), from, to, currentTimeMs, travelTime);
            
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
        realTimeArbiter.advanceTime(currentTimeMs, *this);
    }

    /**
     * @brief נקראת על ידי הארביטר כאשר מלך מוכה.
     */
    void signalGameOver() {
        isGameOver = true;
    }

    
    /**
 * @brief מבקש להתחיל קפיצה עבור הכלי שנמצא במיקום הנתון.
 *
 * הקפיצה אינה מזיזה את הכלי.
 * היא רק מסמנת אותו כ-AIRBORNE למשך זמן קבוע.
 *
 * @param position מיקום הכלי המבקש לקפוץ.
 * @return MoveResult המציין האם הקפיצה התקבלה.
 */
MoveResult requestJump(const Position& position);
};