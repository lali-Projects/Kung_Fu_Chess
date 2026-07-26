#include "RoomFactory.hpp"


#include "Room.hpp"
#include "GameContext.hpp"
#include "GameSession.hpp"



#include "GameController.hpp"
#include "GameSnapshotBuilder.hpp"




//================================================
// Constructor
//================================================

RoomFactory::RoomFactory(
    EventBus& eventBus)
:
m_eventBus(eventBus)
{
}






//================================================
// Create Room
//================================================

std::unique_ptr<Room>
RoomFactory::createRoom(
    const std::string& roomId)
{

    if(roomId.empty())
    {
        return nullptr;
    }



    /*
        GameContext owns:

        - Board
        - RuleEngine
        - RealTimeArbiter
        - GameEngine
        - GameController
        - GameSnapshotBuilder
    */

    auto context =
        std::make_unique<GameContext>();



    if(!context)
    {
        return nullptr;
    }





    /*
        GameSession does not own these objects.

        It only receives references.
    */

    auto session =
        std::make_unique<GameSession>(
            roomId,
            context->getController(),
            context->getSnapshotBuilder(),
            m_eventBus);




    if(!session)
    {
        return nullptr;
    }





    return std::make_unique<Room>(
        roomId,
        std::move(context),
        std::move(session));

}