#pragma once

#include "Board.hpp"
#include "Motion.hpp"
#include "Jump.hpp"
#include "Rest.hpp"

#include <optional>
#include <memory>
#include <vector>


class GameEngine;


class RealTimeArbiter
{

private:

    Board& board;


    // רק Motion אחד יכול להיות פעיל
    std::optional<Motion> activeMotion;


    // רק Jump אחד יכול להיות פעיל
    std::optional<Jump> activeJump;


    // מספר כלים יכולים לנוח בו-זמנית, ולכן זהו אוסף ולא ערך יחיד
    std::vector<Rest> activeRests;



private:


    void processMotionCompletion(
        GameEngine& engine);


    void processJumpCompletion();


    void processRestCompletion(
        const Rest& rest);



    void handlePawnPromotion(
        std::shared_ptr<Piece> piece,
        const Position& pos);



public:


    explicit RealTimeArbiter(Board& board);



    //---------------------------------
    // Motion
    //---------------------------------

    bool startMotion(
        std::shared_ptr<Piece> piece,
        Position src,
        Position dst,
        int startTime);



    void handleMotionLogic(
        int currentTime,
        GameEngine& engine);



    void resolveMotion(
        GameEngine& engine);



    void executeStandardMove(
        GameEngine& engine);



    //---------------------------------
    // Jump
    //---------------------------------


    bool startJump(
        std::shared_ptr<Piece> piece,
        int startTime,
        int duration);



    void handleJumpLogic(
        int currentTime);



    bool isCollisionWithJump(
        const Position& pos) const;



    void handleJumpCollision(
        std::shared_ptr<Piece> movingPiece);



    //---------------------------------
    // Rest
    //---------------------------------

    // התחלת מנוחה עבור כלי, בעקבות סיום תנועה או קפיצה.
    // פרטי (private-like) לשימוש פנימי בלבד של הארביטר, אך נחשף כ-public
    // כדי לאפשר בדיקות יחידה ישירות על מנגנון המנוחה.
 bool startRest(
    std::shared_ptr<Piece> piece,
    PieceState restState,
    PieceState nextState,
    int startTime);


    void handleRestLogic(
        int currentTime);


    bool hasActiveRestFor(
        int pieceId) const;


    int getRestStartTime(
        int pieceId) const;


    int getRestFinishTime(int pieceId) const;


    //---------------------------------
    // Time
    //---------------------------------

    void advanceTime(
        int currentTime,
        GameEngine& engine);



    //---------------------------------
    // Queries
    //---------------------------------

    bool hasActiveMotion() const;


    bool hasActiveJump() const;


    bool hasActiveAnimation(
        int pieceId) const;


    int getAnimationStartTime(
        int pieceId) const;
       void finishMotion();

       //---------------------------------
// Motion animation queries
//---------------------------------

bool hasActiveMotionFor(
    int pieceId) const;


Position getMotionStart(
    int pieceId) const;


Position getMotionDestination(
    int pieceId) const;


int getMotionStartTime(
    int pieceId) const;


int getMotionFinishTime(int pieceId) const;
};