#pragma once

#include "Board.hpp"
#include "Motion.hpp"
#include "Jump.hpp"
#include "Rest.hpp"

#include <optional>
#include <memory>
#include <vector>

class GameEngine;

/**
 * @class RealTimeArbiter
 * @brief מנהל את כלל האירועים והפעולות בזמן אמת במשחק (תנועה, קפיצה, מנוחה).
 */
class RealTimeArbiter {
private:
    Board& board;

    std::optional<Motion> activeMotion;
    std::optional<Jump> activeJump;
    std::vector<Rest> activeRests;

private:
    /** @brief מבצע את ניקוי הלוגיקה בסיום תנועה. */
    void processMotionCompletion(GameEngine& engine);
    
    /** @brief מבצע את ניקוי הלוגיקה בסיום קפיצה. */
    void processJumpCompletion();
    
    /** @brief מבצע את ניקוי הלוגיקה בסיום מצב מנוחה. */
    void processRestCompletion(const Rest& rest);
    
    /** @brief מבצע לוגיקת קידום חייל (Pawn Promotion) במידה והגיע לקצה הלוח. */
    void handlePawnPromotion(std::shared_ptr<Piece> piece, const Position& pos);

public:
    /** @brief בנאי המאתחל את הארביטר עם הלוח הנוכחי. */
    explicit RealTimeArbiter(Board& board);

    //---------------------------------
    // Motion
    //---------------------------------
    
    /** @brief מתחיל פעולת תנועה (Motion) עבור כלי מסוים. */
    bool startMotion(std::shared_ptr<Piece> piece, Position src, Position dst, int startTime);
    
    /** @brief מעדכן את הלוגיקה של התנועה בהתאם לזמן המשחק הנוכחי. */
    void handleMotionLogic(int currentTime, GameEngine& engine);
    
    /** @brief פותר סופית את מצב התנועה ומעדכן את הלוח. */
    void resolveMotion(GameEngine& engine);
    
    /** @brief מוציא לפועל את תנועת הכלי על הלוח. */
    void executeStandardMove(GameEngine& engine);
    
    /** @brief מסיים ידנית את מצב התנועה. */
    void finishMotion();

    //---------------------------------
    // Jump
    //---------------------------------
    
    /** @brief מתחיל פעולת קפיצה (Jump) עבור כלי מסוים. */
    bool startJump(std::shared_ptr<Piece> piece, int startTime, int duration);
    
    /** @brief מעדכן את הלוגיקה של הקפיצה. */
    void handleJumpLogic(int currentTime);
    
    /** @brief בודק האם מיקום מסוים נמצא תחת התנגשות של קפיצה פעילה. */
    bool isCollisionWithJump(const Position& pos) const;
    
    /** @brief מטפל בלוגיקה של התנגשות במהלך ביצוע קפיצה. */
    void handleJumpCollision(std::shared_ptr<Piece> movingPiece);

    //---------------------------------
    // Rest
    //---------------------------------
    
    /** @brief מתחיל מצב מנוחה (Rest) עבור כלי מסוים. */
    bool startRest(std::shared_ptr<Piece> piece, PieceState restState, PieceState nextState, int startTime);
    
    /** @brief מעדכן את הלוגיקה של מנוחות פעילות. */
    void handleRestLogic(int currentTime);
    
    /** @brief בודק האם קיימת מנוחה פעילה לכלי בעל מזהה מסוים. */
    bool hasActiveRestFor(int pieceId) const;
    
    /** @brief מחזיר את זמן התחלת המנוחה של כלי. */
    int getRestStartTime(int pieceId) const;
    
    /** @brief מחזיר את זמן סיום המנוחה של כלי. */
    int getRestFinishTime(int pieceId) const;

    //---------------------------------
    // Time
    //---------------------------------
    
    /** @brief מקדם את זמן המשחק ומפעיל את עדכון כלל המנגנונים הפעילים. */
    void advanceTime(int currentTime, GameEngine& engine);

    //---------------------------------
    // Queries
    //---------------------------------
    
    /** @brief מחזיר האם קיימת תנועה פעילה בלוח. */
    bool hasActiveMotion() const;
    
    /** @brief מחזיר האם קיימת קפיצה פעילה בלוח. */
    bool hasActiveJump() const;
    
    /** @brief מחזיר האם קיימת אנימציה פעילה לכלי מסוים. */
    bool hasActiveAnimation(int pieceId) const;
    
    /** @brief מחזיר את זמן התחלת האנימציה של כלי. */
    int getAnimationStartTime(int pieceId) const;

    //---------------------------------
    // Motion animation queries
    //---------------------------------
    
    /** @brief מחזיר האם קיימת תנועה פעילה לכלי ספציפי. */
    bool hasActiveMotionFor(int pieceId) const;
    
    /** @brief מחזיר את מיקום ההתחלה של תנועת כלי. */
    Position getMotionStart(int pieceId) const;
    
    /** @brief מחזיר את מיקום היעד של תנועת כלי. */
    Position getMotionDestination(int pieceId) const;
    
    /** @brief מחזיר את זמן התחלת התנועה של כלי. */
    int getMotionStartTime(int pieceId) const;
    
    /** @brief מחזיר את זמן סיום התנועה של כלי. */
    int getMotionFinishTime(int pieceId) const;
};