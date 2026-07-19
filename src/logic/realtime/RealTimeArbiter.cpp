#include "RealTimeArbiter.hpp"

#include "GameEngine.hpp"
#include "GameConfig.hpp"

#include <cmath>
#include <algorithm>



RealTimeArbiter::RealTimeArbiter(Board& b)
:
board(b)
{
}



//================================================
// Motion start
//================================================


bool RealTimeArbiter::startMotion(
    std::shared_ptr<Piece> piece,
    Position source,
    Position destination,
    int startTime)
{


    if(!piece)
        return false;



    // אין שתי תנועות במקביל
    if(activeMotion.has_value())
        return false;



    // כלי קופץ לא יכול לזוז
    if(piece->getState() == PieceState::AIRBORNE)
        return false;



    int rowDistance =
        std::abs(
            destination.getRow()
            -
            source.getRow());


    int colDistance =
        std::abs(
            destination.getCol()
            -
            source.getCol());


    int distance =
        std::max(
            rowDistance,
            colDistance);



    int travelTime =
        distance *
        GameConfig::DEFAULT_TRAVEL_TIME_MS;



    piece->setState(
        PieceState::MOVING);



    activeMotion.emplace(
        piece,
        source,
        destination,
        startTime,
        travelTime);



    return true;
}



//================================================
// Jump start
//================================================


bool RealTimeArbiter::startJump(
    std::shared_ptr<Piece> piece,
    int startTime,
    int duration)
{


    if(!piece)
        return false;



    // אין שתי קפיצות
    if(activeJump.has_value())
        return false;



    // כלי בתנועה לא קופץ
    if(piece->getState()
        ==
        PieceState::MOVING)
    {
        return false;
    }



    piece->setState(
        PieceState::AIRBORNE);



    activeJump.emplace(
        piece,
        piece->getPosition(),
        startTime,
        duration);



    return true;
}



//================================================
// Time update
//================================================


void RealTimeArbiter::advanceTime(
    int currentTime,
    GameEngine& engine)
{

    handleMotionLogic(
        currentTime,
        engine);



    handleJumpLogic(
        currentTime);

}



//================================================
// Motion logic
//================================================


void RealTimeArbiter::handleMotionLogic(
    int currentTime,
    GameEngine& engine)
{
    if(!activeMotion.has_value())
        return;


    if(activeMotion->hasArrived(currentTime))
    {

        resolveMotion(engine);


        finishMotion();


        activeMotion.reset();
    }
}
void RealTimeArbiter::finishMotion()
{
    if(!activeMotion.has_value())
        return;


    auto piece =
        activeMotion->movingPiece;


    if(piece)
    {
        piece->setState(
            PieceState::IDLE);
    }
}



//================================================
// Jump logic
//================================================


void RealTimeArbiter::handleJumpLogic(
    int currentTime)
{


    if(!activeJump)
        return;



    if(activeJump->hasFinished(currentTime))
    {

        processJumpCompletion();

    }

}



//================================================
// Motion resolution
//================================================


void RealTimeArbiter::resolveMotion(
    GameEngine& engine)
{


    Position destination =
        activeMotion->destination;



    if(isCollisionWithJump(destination))
    {

        handleJumpCollision(
            activeMotion->movingPiece);

    }
    else
    {

        executeStandardMove(engine);

    }

}



//================================================
// Normal move
//================================================


void RealTimeArbiter::executeStandardMove(
    GameEngine& engine)
{


    auto movingPiece =
        activeMotion->movingPiece;



    Position destination =
        activeMotion->destination;



    auto target =
        board.getPieceAt(destination);



    if(target)
    {


        if(target->getType()
            ==
            PieceType::KING)
        {
            engine.signalGameOver();
        }



        board.removePiece(
            destination);

    }



    board.movePiece(
        activeMotion->source,
        destination);



    movingPiece->setState(
        PieceState::IDLE);



    handlePawnPromotion(
        movingPiece,
        destination);

}



//================================================
// Jump collision
//================================================


bool RealTimeArbiter::isCollisionWithJump(
    const Position& pos) const
{

    return
        activeJump.has_value()
        &&
        activeJump->getPosition()
        ==
        pos;

}



void RealTimeArbiter::handleJumpCollision(
    std::shared_ptr<Piece> movingPiece)
{

    if(!movingPiece)
        return;



    /*
       הכלי שמגיע נמחק.
       הכלי שבקפיצה נשאר.
    */


    board.removePiece(
        activeMotion->source);



    movingPiece->setState(
        PieceState::IDLE);

}



//================================================
// Jump finish
//================================================


void RealTimeArbiter::processJumpCompletion()
{


    if(!activeJump)
        return;



    auto piece =
        activeJump->getPiece();



    if(piece)
    {
        piece->setState(
            PieceState::IDLE);
    }



    activeJump.reset();

}



//================================================
// Pawn promotion
//================================================


void RealTimeArbiter::handlePawnPromotion(
    std::shared_ptr<Piece> piece,
    const Position& pos)
{


    if(!piece)
        return;



    if(piece->getType()
        !=
        PieceType::PAWN)
        return;



    int lastRow =
        piece->getSide()
        ==
        Side::WHITE
        ?
        0
        :
        board.getRows()-1;



    if(pos.getRow()==lastRow)
    {
        piece->setType(
            PieceType::QUEEN);
    }

}



//================================================
// Queries
//================================================


bool RealTimeArbiter::hasActiveMotion() const
{
    return activeMotion.has_value();
}



bool RealTimeArbiter::hasActiveJump() const
{
    return activeJump.has_value();
}



bool RealTimeArbiter::hasActiveAnimation(
    int pieceId) const
{


    if(activeMotion)
    {
        if(activeMotion->movingPiece->getId()
            ==
            pieceId)
            return true;
    }



    if(activeJump)
    {
        if(activeJump->getPiece()->getId()
            ==
            pieceId)
            return true;
    }



    return false;
}




int RealTimeArbiter::getAnimationStartTime(
    int pieceId) const
{


    if(activeMotion)
    {

        if(activeMotion->movingPiece->getId()
            ==
            pieceId)
        {
            return activeMotion->startTime;
        }

    }



    if(activeJump)
    {

        if(activeJump->getPiece()->getId()
            ==
            pieceId)
        {
            return activeJump->getStartTime();
        }

    }



    return 0;
}

bool RealTimeArbiter::hasActiveMotionFor(
    int pieceId) const
{

    if(!activeMotion.has_value())
        return false;


    if(!activeMotion->movingPiece)
        return false;


    return activeMotion->movingPiece->getId()
            ==
           pieceId;
}

Position RealTimeArbiter::getMotionStart(
    int pieceId) const
{

    if(hasActiveMotionFor(pieceId))
    {
        return activeMotion->source;
    }


    return Position(-1,-1);
}

Position RealTimeArbiter::getMotionDestination(
    int pieceId) const
{

    if(hasActiveMotionFor(pieceId))
    {
        return activeMotion->destination;
    }


    return Position(-1,-1);
}

int RealTimeArbiter::getMotionStartTime(
    int pieceId) const
{

    if(hasActiveMotionFor(pieceId))
    {
        return activeMotion->startTime;
    }


    return 0;
}

int RealTimeArbiter::getMotionFinishTime(
    int pieceId) const
{

    if(hasActiveMotionFor(pieceId))
    {
        return activeMotion->finishTime;
    }


    return 0;
}