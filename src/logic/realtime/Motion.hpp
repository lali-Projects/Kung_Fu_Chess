#pragma once

#include <memory>

#include "Piece.hpp"
#include "Position.hpp"



/**
 * @class Motion
 * @brief מייצג תנועה פעילה של כלי בזמן אמת.
 *
 * Motion אינו מזיז כלי.
 * Motion אינו משנה לוח.
 *
 * הוא רק מחזיק מידע:
 * - איזה כלי נע
 * - מאיפה לאן
 * - זמני התחלה וסיום
 */
class Motion
{

private:

    std::shared_ptr<Piece> movingPiece;


    Position source;


    Position destination;


    int startTime;


    int finishTime;



public:


    /**
     * @brief יצירת Motion חדש.
     *
     * @param piece הכלי שנע
     * @param src מיקום התחלה
     * @param dst מיקום יעד
     * @param start זמן התחלה
     * @param travelTime זמן נסיעה
     */
    Motion(
        std::shared_ptr<Piece> piece,
        Position src,
        Position dst,
        int start,
        int travelTime)
        :
        movingPiece(piece),
        source(src),
        destination(dst),
        startTime(start),
        finishTime(start + travelTime)
    {
    }



    /**
     * @brief האם התנועה הסתיימה.
     */
    bool hasArrived(
        int currentTime) const
    {
        return currentTime >= finishTime;
    }



    /**
     * @brief קבלת הכלי שבתנועה.
     */
    std::shared_ptr<Piece> getPiece() const
    {
        return movingPiece;
    }



    /**
     * @brief מיקום התחלה.
     */
    Position getSource() const
    {
        return source;
    }



    /**
     * @brief מיקום יעד.
     */
    Position getDestination() const
    {
        return destination;
    }



    /**
     * @brief זמן התחלה.
     */
    int getStartTime() const
    {
        return startTime;
    }



    /**
     * @brief זמן סיום.
     */
    int getFinishTime() const
    {
        return finishTime;
    }

};