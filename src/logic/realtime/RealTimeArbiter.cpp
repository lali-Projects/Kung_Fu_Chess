#include "RealTimeArbiter.hpp"
#include "GameEngine.hpp"
#include "GameConfig.hpp"
#include <iostream>
#include <cmath>
#include <algorithm>

// בנאי המחלקה: מאתחל את הבורר עם הפניה ללוח המשחק.
RealTimeArbiter::RealTimeArbiter(Board& b) : board(b) {}

//================================================
// Motion start
//================================================

// בודק תנאים ומתחיל תנועה רגילה של כלי מנקודה לנקודה.
bool RealTimeArbiter::startMotion(std::shared_ptr<Piece> piece, Position source, Position destination, int startTime) {
    if (!piece) return false;

    // אין שתי תנועות במקביל
    if (activeMotion.has_value()) return false;

    // כלי קופץ לא יכול לזוז
    if (piece->getState() == PieceState::AIRBORNE) return false;

    // כלי שנח לא יכול לזוז
    if (piece->getState() == PieceState::LONG_REST || piece->getState() == PieceState::SHORT_REST) {
        return false;
    }

    int rowDistance = std::abs(destination.getRow() - source.getRow());
    int colDistance = std::abs(destination.getCol() - source.getCol());
    int distance = std::max(rowDistance, colDistance);
    int travelTime = distance * GameConfig::DEFAULT_TRAVEL_TIME_MS;

    piece->setState(PieceState::MOVING);
    activeMotion.emplace(piece, source, destination, startTime, travelTime);

    return true;
}

//================================================
// Jump start
//================================================

// בודק תנאים ומתחיל אנימציית קפיצה של כלי.
bool RealTimeArbiter::startJump(std::shared_ptr<Piece> piece, int startTime, int duration) {
    if (!piece) return false;

    if (activeJump.has_value()) return false;

    if (piece->getState() == PieceState::MOVING) return false;

    if (piece->getState() == PieceState::LONG_REST || piece->getState() == PieceState::SHORT_REST) {
        return false;
    }

    piece->setState(PieceState::AIRBORNE);
    activeJump.emplace(piece, piece->getPosition(), startTime, duration);

    return true;
}

//================================================
// Time update
//================================================

// פונקציית העדכון הראשית המריצה את לוגיקת הזמן של כלל המערכת.
void RealTimeArbiter::advanceTime(int currentTime, GameEngine& engine) {
    handleMotionLogic(currentTime, engine);
    handleJumpLogic(currentTime);
    handleRestLogic(currentTime);
}

//================================================
// Motion logic
//================================================

// בודקת האם תנועה פעילה הגיעה ליעדה ומבצעת את סיומה.
void RealTimeArbiter::handleMotionLogic(int currentTime, GameEngine& engine) {
    if (!activeMotion.has_value()) return;

    if (activeMotion->hasArrived(currentTime)) {
        resolveMotion(engine);
        finishMotion();
        activeMotion.reset();
    }
}

// מבצעת פעולות סיום תנועה ומעבירה את הכלי למצב מנוחה.
void RealTimeArbiter::finishMotion() {
    if (!activeMotion.has_value()) return;

    auto piece = activeMotion->movingPiece;
    
    std::cout << "finishMotion piece id=" << piece->getId() 
              << " state before=" << static_cast<int>(piece->getState()) << std::endl;

    if (!piece) return;

    if (piece->getState() == PieceState::MOVING) {
        startRest(piece, PieceState::LONG_REST, PieceState::IDLE, activeMotion->finishTime);
        std::cout << "LONG_REST STARTED for piece " << piece->getId() << std::endl;
    }
}

//================================================
// Jump logic
//================================================

// בודקת האם קפיצה פעילה הסתיימה ומפעילה את תהליך הסיום.
void RealTimeArbiter::handleJumpLogic(int currentTime) {
    if (!activeJump) return;

    if (activeJump->hasFinished(currentTime)) {
        processJumpCompletion();
    }
}

//================================================
// Motion resolution
//================================================

// מחליטה האם התנועה היא רגילה או התנגשות עם כלי קופץ.
void RealTimeArbiter::resolveMotion(GameEngine& engine) {
    Position destination = activeMotion->destination;

    if (isCollisionWithJump(destination)) {
        handleJumpCollision(activeMotion->movingPiece);
    } else {
        executeStandardMove(engine);
    }
}

//================================================
// Normal move
//================================================

// מבצעת הזזת כלי בפועל על הלוח וטיפול באכילת כלים.
void RealTimeArbiter::executeStandardMove(GameEngine& engine) {
    auto movingPiece = activeMotion->movingPiece;
    Position destination = activeMotion->destination;
    auto target = board.getPieceAt(destination);

    if (target) {
        if (target->getType() == PieceType::KING) {
            engine.signalGameOver();
        }
        board.removePiece(destination);
    }

    board.movePiece(activeMotion->source, destination);

    handlePawnPromotion(movingPiece, destination);
}

//================================================
// Jump collision
//================================================

// בודקת התנגשות בין מיקום מסוים לבין כלי שנמצא בקפיצה.
bool RealTimeArbiter::isCollisionWithJump(const Position& pos) const {
    return activeJump.has_value() && activeJump->getPosition() == pos;
}

// מטפלת בהתנגשות: מסירה את הכלי שהיה בתנועה.
void RealTimeArbiter::handleJumpCollision(std::shared_ptr<Piece> movingPiece) {
    if (!movingPiece) return;

    // הכלי שמגיע נמחק, הכלי שבקפיצה נשאר
    board.removePiece(activeMotion->source);
    movingPiece->setState(PieceState::IDLE);
}

//================================================
// Jump finish
//================================================

// מנקה את מצב הקפיצה ומעבירה את הכלי למנוחה.
void RealTimeArbiter::processJumpCompletion() {
    if (!activeJump) return;

    auto piece = activeJump->getPiece();
    if (piece) {
        startRest(piece, PieceState::SHORT_REST, PieceState::IDLE, 
                  activeJump->getStartTime() + activeJump->getDuration());
    }
    activeJump.reset();
}

//================================================
// Pawn promotion
//================================================

// בודקת הגעה של חייל לשורת ההכתרה ומשדרגת אותו למלכה.
void RealTimeArbiter::handlePawnPromotion(std::shared_ptr<Piece> piece, const Position& pos) {
    if (!piece || piece->getType() != PieceType::PAWN) return;

    int lastRow = (piece->getSide() == Side::WHITE) ? 0 : board.getRows() - 1;

    if (pos.getRow() == lastRow) {
        piece->setType(PieceType::QUEEN);
    }
}

//================================================
// Queries
//================================================

// בודקת האם יש תנועה פעילה כרגע.
bool RealTimeArbiter::hasActiveMotion() const {
    return activeMotion.has_value();
}

// בודקת האם יש קפיצה פעילה כרגע.
bool RealTimeArbiter::hasActiveJump() const {
    return activeJump.has_value();
}

// בודקת האם כלי ספציפי נמצא באנימציה כלשהי (תנועה, קפיצה או מנוחה).
bool RealTimeArbiter::hasActiveAnimation(int pieceId) const {
    if (activeMotion && activeMotion->movingPiece->getId() == pieceId) return true;
    if (activeJump && activeJump->getPiece()->getId() == pieceId) return true;
    if (hasActiveRestFor(pieceId)) return true;
    return false;
}

// מחזירה את זמן התחלת האנימציה עבור כלי ספציפי.
int RealTimeArbiter::getAnimationStartTime(int pieceId) const {
    if (activeMotion && activeMotion->movingPiece->getId() == pieceId) return activeMotion->startTime;
    if (activeJump && activeJump->getPiece()->getId() == pieceId) return activeJump->getStartTime();
    if (hasActiveRestFor(pieceId)) return getRestStartTime(pieceId);
    return 0;
}

// בודקת האם תנועה ספציפית שייכת לכלי מסוים.
bool RealTimeArbiter::hasActiveMotionFor(int pieceId) const {
    return activeMotion.has_value() && activeMotion->movingPiece && activeMotion->movingPiece->getId() == pieceId;
}

// מחזירה את נקודת המקור של תנועת כלי.
Position RealTimeArbiter::getMotionStart(int pieceId) const {
    return hasActiveMotionFor(pieceId) ? activeMotion->source : Position(-1, -1);
}

// מחזירה את נקודת היעד של תנועת כלי.
Position RealTimeArbiter::getMotionDestination(int pieceId) const {
    return hasActiveMotionFor(pieceId) ? activeMotion->destination : Position(-1, -1);
}

// מחזירה את זמן התחלת התנועה של כלי.
int RealTimeArbiter::getMotionStartTime(int pieceId) const {
    return hasActiveMotionFor(pieceId) ? activeMotion->startTime : 0;
}

// מחזירה את זמן סיום התנועה של כלי.
int RealTimeArbiter::getMotionFinishTime(int pieceId) const {
    return hasActiveMotionFor(pieceId) ? activeMotion->finishTime : 0;
}

//================================================
// Rest start
//================================================

// מעבירה כלי למצב מנוחה.
bool RealTimeArbiter::startRest(std::shared_ptr<Piece> piece, PieceState restState, PieceState nextState, int startTime) {
    if (!piece) return false;

    int duration = (restState == PieceState::LONG_REST) ? GameConfig::LONG_REST_DURATION_MS : GameConfig::SHORT_REST_DURATION_MS;

    std::cout << "startRest: piece=" << piece->getId() << " state=" << static_cast<int>(restState) 
              << " duration=" << duration << " start=" << startTime << std::endl;
    
    piece->setState(restState);
    activeRests.emplace_back(piece, restState, nextState, startTime, duration);

    return true;
}

//================================================
// Rest logic
//================================================

// בודקת את כל המנוחות הפעילות ומסיימת את אלו שזמנן עבר.
void RealTimeArbiter::handleRestLogic(int currentTime) {
    for (auto it = activeRests.begin(); it != activeRests.end();) {
        std::cout << "REST CHECK piece=" << it->getPiece()->getId() << " start=" << it->getStartTime() 
                  << " duration=" << it->getDuration() << " current=" << currentTime << std::endl;
        
        if (it->hasFinished(currentTime)) {
            std::cout << "REST FINISHED piece=" << it->getPiece()->getId() << " current=" << currentTime << std::endl;
            processRestCompletion(*it);
            it = activeRests.erase(it);
        } else {
            ++it;
        }
    }
}

//================================================
// Rest finish
//================================================

// מסיימת את מצב המנוחה של הכלי ומחזירה אותו למצבו הפעיל.
void RealTimeArbiter::processRestCompletion(const Rest& rest) {
    auto piece = rest.getPiece();
    if (!piece) return;

    std::cout << "Rest completed piece=" << piece->getId() << " old state=" << static_cast<int>(piece->getState()) << std::endl;
    piece->setState(rest.getNextState());
}

//================================================
// Rest queries
//================================================

// בודקת האם לכלי מסוים יש מנוחה פעילה.
bool RealTimeArbiter::hasActiveRestFor(int pieceId) const {
    for (const auto& rest : activeRests) {
        if (rest.getPiece() && rest.getPiece()->getId() == pieceId) return true;
    }
    return false;
}

// מחזירה את זמן התחלת המנוחה של כלי.
int RealTimeArbiter::getRestStartTime(int pieceId) const {
    for (const auto& rest : activeRests) {
        if (rest.getPiece() && rest.getPiece()->getId() == pieceId) return rest.getStartTime();
    }
    return 0;
}

// מחזירה את זמן סיום המנוחה של כלי.
int RealTimeArbiter::getRestFinishTime(int pieceId) const {
    for (const auto& rest : activeRests) {
        if (rest.getPiece() && rest.getPiece()->getId() == pieceId) return rest.getStartTime() + rest.getDuration();
    }
    return 0;
}