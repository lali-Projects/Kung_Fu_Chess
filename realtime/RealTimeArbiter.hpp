#pragma once
#include "Board.hpp"
#include "Motion.hpp"
#include <optional>
#include <memory>
#include "Jump.hpp"

class GameEngine; // הצהרה מקדימה

class RealTimeArbiter {
private:
    Board& board;
    std::optional<Motion> activeMotion;
    /**
    * @brief הקפיצה הפעילה כרגע במשחק.
    *
   * המשחק מאפשר לכל היותר קפיצה אחת פעילה,
   * בדומה לכך שמותרת תנועה אחת בלבד.
   */
   std::optional<Jump> activeJump;
    // פונקציות עזר פרטיות לניהול תהליך התנועה
    void processMotionCompletion(GameEngine& engine);
    void handlePawnPromotion(std::shared_ptr<Piece> piece, const Position& pos);
    /**
    * @brief מסיים קפיצה שהסתיימה.
    *
    * הפונקציה מחזירה את הכלי למצב IDLE
    * ומסירה את הקפיצה הפעילה.
    */
void processJumpCompletion();
public:
    RealTimeArbiter(Board& b) : board(b) {}

    void startMotion(std::shared_ptr<Piece> piece, Position src, Position dst, int startTime, int travelTime);
    
    // בודקת האם הגיע זמן ביצוע התנועה ועדכון הלוח
    void advanceTime(int currentTime, GameEngine& engine);

    bool hasActiveMotion() const;
    /**
    * @brief מתחיל קפיצה חדשה.
    *
    * הקפיצה אינה מזיזה את הכלי ואינה משנה את הלוח.
    * היא רק מסמנת את הכלי כ-AIRBORNE ויוצרת אובייקט Jump.
    *
    * @param piece הכלי שקופץ.
    * @param startTime זמן תחילת הקפיצה.
    * @param jumpDuration משך הקפיצה במילישניות.
    */
    void startJump(std::shared_ptr<Piece> piece,
               int startTime,
               int jumpDuration);
    void handleMotionLogic(int currentTime, GameEngine& engine);
    void handleJumpLogic(int currentTime);
    void resolveMotion(GameEngine& engine);
    bool isCollisionWithJump(const Position& pos) const;
    void executeStandardMove(GameEngine& engine);
    void handleJumpCollision(std::shared_ptr<Piece> movingPiece);
};