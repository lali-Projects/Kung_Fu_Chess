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
        std::cout
        << "SNAPSHOT BUILDER piece="
        << piece.id
        << " state="
        << static_cast<int>(piece.state)
        << std::endl;
        piece.animationStartTime = gameEngine.getAnimationStartTime(piece.id);
                

    }



    //---------------------------------
    // Selection
    //---------------------------------

    snapshot.setSelectedPosition(
        controller.getSelectedPosition());


std::cout << "SNAPSHOT BUILD FINISHED"
          << std::endl;
    return snapshot;
}