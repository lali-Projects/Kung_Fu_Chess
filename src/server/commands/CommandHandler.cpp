#include "CommandHandler.hpp"


#include "ClickCommand.hpp"

#include "RoomManager.hpp"
#include "Room.hpp"

#include "GameSession.hpp"
#include "PlayerSession.hpp"





//================================================
// Constructor
//================================================

CommandHandler::CommandHandler(
    RoomManager& roomManager)
:
m_roomManager(roomManager)
{
}






//================================================
// Handle Command
//================================================

MoveResult CommandHandler::handle(
    PlayerSession& player,
    const ClickCommand& command)
{

    //---------------------------------
    // Player must belong to room
    //---------------------------------

    if(!player.hasRoom())
    {
        return
        {
            false,
            "player_has_no_room"
        };
    }



    //---------------------------------
    // Find room
    //---------------------------------

    Room* room =
        m_roomManager.getRoom(
            player.getRoomId());



    if(!room)
    {
        return
        {
            false,
            "room_not_found"
        };
    }





    //---------------------------------
    // Forward command
    //---------------------------------

    return
        room->getSession()
            .handleClick(
                player,
                command);

}