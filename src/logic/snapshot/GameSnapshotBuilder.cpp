#include "GameSnapshotBuilder.hpp"


GameSnapshotBuilder::GameSnapshotBuilder(const GameEngine& engine, const GameController& controller)
:
gameEngine(engine),
controller(controller)
{
}


GameSnapshot GameSnapshotBuilder::build() const
{
    GameSnapshot snapshot = gameEngine.getSnapshot();

    snapshot.setSelectedPosition(controller.getSelectedPosition());

    return snapshot;
}