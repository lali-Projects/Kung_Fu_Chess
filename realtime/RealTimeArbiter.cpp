#include "RealTimeArbiter.hpp"
#include "GameEngine.hpp"

void RealTimeArbiter::startMotion(std::shared_ptr<Piece> piece, Position src, Position dst, int startTime, int travelTime) {
    activeMotion.emplace(piece, src, dst, startTime, travelTime);
}

/**
 * @brief מנהלת את זרימת הזמן ובודקת האם הגיע הזמן לבצע סיום תנועה.
 */
void RealTimeArbiter::advanceTime(int currentTime, GameEngine& engine) {
    if (!activeMotion.has_value() || !activeMotion->hasArrived(currentTime)) {
        return;
    }

    processMotionCompletion(engine);
    activeMotion.reset();
}

/**
 * @brief מבצעת את כל הפעולות הנדרשות בעת הגעת כלי ליעדו.
 */
void RealTimeArbiter::processMotionCompletion(GameEngine& engine) {
    Position destination = activeMotion->destination;
    std::shared_ptr<Piece> movingPiece = activeMotion->movingPiece;
    std::shared_ptr<Piece> targetPiece = board.getPieceAt(destination);

    // 1. בדיקת סיום משחק (הכאת מלך)
    if (targetPiece && targetPiece->getType() == PieceType::KING) {
        engine.signalGameOver();
    }

    // 2. ביצוע הכאה בלוח
    if (targetPiece) {
        board.removePiece(destination);
    }

    // 3. ביצוע המהלך עצמו
    board.movePiece(activeMotion->source, destination);

    // 4. טיפול בקידום רגלי (Promotion)
    handlePawnPromotion(movingPiece, destination);
}

/**
 * @brief בודקת האם רגלי הגיע לשורת הסיום ומקדמת אותו למלכה במידת הצורך.
 */
void RealTimeArbiter::handlePawnPromotion(std::shared_ptr<Piece> piece, const Position& pos) {
    if (piece && piece->getType() == PieceType::PAWN) {
        int lastRow = (piece->getSide() == Side::WHITE) ? 0 : board.getRows() - 1;
        
        if (pos.getRow() == lastRow) {
            piece->setType(PieceType::QUEEN);
        }
    }
}

bool RealTimeArbiter::hasActiveMotion() const {
    return activeMotion.has_value();
}