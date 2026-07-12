#include "RealTimeArbiter.hpp"
#include "GameEngine.hpp" // הכללה מותרת כאן!

void RealTimeArbiter::startMotion(std::shared_ptr<Piece> piece, Position src, Position dst, int startTime, int travelTime) {
    activeMotion.emplace(piece, src, dst, startTime, travelTime);
}

void RealTimeArbiter::advanceTime(int currentTime, GameEngine& engine) {
    if (activeMotion.has_value() && activeMotion->hasArrived(currentTime)) {
        
        auto targetPiece = board.getPieceAt(activeMotion->destination);
        
        // זיהוי הכאת מלך -> איתות למנוע
        if (targetPiece && targetPiece->getType() == PieceType::KING) {
            engine.signalGameOver();
        }

        // ביצוע הכאה (אם יש כלי ביעד)
        if (targetPiece) {
            board.removePiece(activeMotion->destination);
        }

        // הזזת הכלי
        board.movePiece(activeMotion->source, activeMotion->destination);
        activeMotion.reset();
    }
}

bool RealTimeArbiter::hasActiveMotion() const {
    return activeMotion.has_value();
}