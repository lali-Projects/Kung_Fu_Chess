#include "RealTimeArbiter.hpp"
#include "GameEngine.hpp"
#include "GameConfig.hpp" // הוספנו את זה כדי לקבל את הקבועים
#include <cmath>
#include <algorithm>

// המימוש החדש: הארביטר הוא "המומחה" שמחשב את זמן התנועה
void RealTimeArbiter::startMotion(std::shared_ptr<Piece> piece, Position source, Position destination, int startTime) {
    // 1. חישוב זמן התנועה עבר לכאן
    int rowDiff = std::abs(destination.getRow() - source.getRow());
    int colDiff = std::abs(destination.getCol() - source.getCol());
    int distance = std::max(rowDiff, colDiff);
    int travelTime = distance * GameConfig::DEFAULT_TRAVEL_TIME_MS;

    // 2. עדכון המצב והתחלת התנועה
    piece->setState(PieceState::MOVING); 
    activeMotion.emplace(piece, source, destination, startTime, travelTime);
}

// ניהול הזמן והבדיקה האם פעולות בזמן אמת הגיעו לסיומן
void RealTimeArbiter::advanceTime(int currentTime, GameEngine& engine) {
    handleMotionLogic(currentTime, engine);
    handleJumpLogic(currentTime);
}

// ניהול תהליך התנועה (Motion)
void RealTimeArbiter::handleMotionLogic(int currentTime, GameEngine& engine) {
    if (activeMotion.has_value() && activeMotion->hasArrived(currentTime)) {
        resolveMotion(engine);
        activeMotion.reset();
    }
}

// ניהול תהליך הקפיצה (Jump)
void RealTimeArbiter::handleJumpLogic(int currentTime) {
    if (activeJump.has_value() && activeJump->hasFinished(currentTime)) {
        processJumpCompletion();
    }
}

// החלטה על אופן סיום התנועה: האם מדובר במהלך תקין או בהתנגשות עם כלי קופץ?
void RealTimeArbiter::resolveMotion(GameEngine& engine) {
    Position destination = activeMotion->destination;
    
    if (isCollisionWithJump(destination)) {
        handleJumpCollision(activeMotion->movingPiece);
    } else {
        executeStandardMove(engine);
    }
}

// בדיקה האם היעד של התנועה תפוס על ידי כלי שנמצא ב"קפיצה" (AIRBORNE)
bool RealTimeArbiter::isCollisionWithJump(const Position& pos) const {
    return activeJump.has_value() && activeJump->getPosition() == pos;
}

// לוגיקה של ביצוע מהלך שחמט רגיל
void RealTimeArbiter::executeStandardMove(GameEngine& engine) {
    Position destination = activeMotion->destination;
    auto movingPiece = activeMotion->movingPiece;
    auto targetPiece = board.getPieceAt(destination);

    // 1. זיהוי הכאת מלך (סיום משחק)
    if (targetPiece && targetPiece->getType() == PieceType::KING) {
        engine.signalGameOver();
    }

    // 2. הסרת הכלי המותקף מהלוח
    if (targetPiece) {
        board.removePiece(destination);
    }

    // 3. עדכון מיקום הכלי בלוח
    board.movePiece(activeMotion->source, destination);

    // 4. בדיקת קידום רגלי למלכה
    handlePawnPromotion(movingPiece, destination);
}

// טיפול מיוחד במקרה של אכילה תוך כדי קפיצה
void RealTimeArbiter::handleJumpCollision(std::shared_ptr<Piece> movingPiece) {
    // הכלי המגיע נמחק מהלוח, הכלי הקופץ נשאר במקומו
    board.removePiece(activeMotion->source);
    // התנועה בטלה (לא מעדכנים את הלוח ליעד כי המגיע הוסר)
}

// בדיקת רגלי בשורת הסיום
void RealTimeArbiter::handlePawnPromotion(std::shared_ptr<Piece> piece, const Position& pos) {
    if (piece && piece->getType() == PieceType::PAWN) {
        int lastRow = (piece->getSide() == Side::WHITE) ? 0 : board.getRows() - 1;
        if (pos.getRow() == lastRow) {
            piece->setType(PieceType::QUEEN);
        }
    }
}


// התחלת מצב AIRBORNE
void RealTimeArbiter::startJump(std::shared_ptr<Piece> piece, int startTime, int jumpDuration) {
    piece->setState(PieceState::AIRBORNE);
    activeJump.emplace(piece, piece->getPosition(), startTime, jumpDuration);
}

// סיום מצב AIRBORNE וחזרה ל-IDLE
void RealTimeArbiter::processJumpCompletion() {
    if (activeJump.has_value()) {
        activeJump->getPiece()->setState(PieceState::IDLE);
        activeJump.reset();
    }
}

bool RealTimeArbiter::hasActiveMotion() const {
    return activeMotion.has_value();
}