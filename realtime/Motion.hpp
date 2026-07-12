#pragma once
#include "Board.hpp"
#include "Piece.hpp"
#include <memory>

class Motion {
public:
    std::shared_ptr<Piece> movingPiece;
    Position source;
    Position destination;
    int finishTime; // הזמן האבסולוטי שבו התנועה תסתיים

    Motion(std::shared_ptr<Piece> piece, Position src, Position dst, int startTime, int travelTime)
        : movingPiece(piece), source(src), destination(dst), finishTime(startTime + travelTime) {}

    // בדיקה האם הזמן הנוכחי עבר את זמן הסיום המתוכנן
    bool hasArrived(int currentTime) const {
        return currentTime >= finishTime;
    }
};