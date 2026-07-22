#include "GameSnapshotBuilder.hpp"



GameSnapshotBuilder::GameSnapshotBuilder(
    const GameEngine& engine,
    const GameController& controller)
:
gameEngine(engine),
controller(controller)
{
}



GameSnapshot GameSnapshotBuilder::build() const
{

    GameSnapshot snapshot =
        gameEngine.getSnapshot();



    //---------------------------------
    // הוספת זמן התחלת אנימציה
    //---------------------------------

    auto& pieces =
        snapshot.getMutablePieces();



    for(auto& piece : pieces)
    {
        piece.animationStartTime = gameEngine.getAnimationStartTime(piece.id);
                
    }



    //---------------------------------
    // Selection
    //---------------------------------

    snapshot.setSelectedPosition(
        controller.getSelectedPosition());

    return snapshot;
}