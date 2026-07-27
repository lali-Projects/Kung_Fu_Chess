#include "CommandHandler.hpp"


#include "ClientConnection.hpp"
#include "Command.hpp"

#include "Position.hpp"
#include "ClickCommand.hpp"

#include "RoomManager.hpp"
#include "Room.hpp"

#include "GameSession.hpp"

#include "PlayerSession.hpp"

#include "AuthService.hpp"


#include <string>




//================================================
// Constructor
//================================================

CommandHandler::CommandHandler(
    RoomManager& roomManager,
    AuthService& authService)
:
m_roomManager(roomManager),
m_authService(authService)
{
}






//================================================
// Main Dispatcher
//================================================

MoveResult CommandHandler::handle(
    ClientConnection& connection,
    const Command& command)
{

    switch(command.getType())
    {


        case CommandType::REGISTER:

            return handleRegister(
                command);



        case CommandType::LOGIN:

            return handleLogin(
                connection,
                command);



        case CommandType::CLICK:

            return handleClick(
                connection,
                command);



        case CommandType::CREATE_ROOM:

            return handleCreateRoom(
                connection,
                command);



        case CommandType::JOIN_ROOM:

            return handleJoinRoom(
                connection,
                command);



        default:

            return
            {
                false,
                "unknown_command"
            };
    }

}








//================================================
// REGISTER
//================================================

MoveResult CommandHandler::handleRegister(
    const Command& command)
{

    const auto& args =
        command.getArgs();



    if(args.size()!=2)
    {
        return
        {
            false,
            "invalid_register_arguments"
        };
    }




    return
        m_authService.registerUser(
            args[0],
            args[1]);

}








//================================================
// LOGIN
//================================================
MoveResult CommandHandler::handleLogin(
    ClientConnection& connection,
    const Command& command)
{
    const auto& args =
        command.getArgs();


    if(args.size()!=2)
    {
        return
        {
            false,
            "invalid_login_arguments"
        };
    }



    auto player =
        m_authService.login(
            args[0],
            args[1]);



    if(!player)
    {
        return
        {
            false,
            "login_failed"
        };
    }



    connection.attachPlayer(
        player);



    return
    {
        true,
        "login_success"
    };
}









//================================================
// CLICK
//================================================

MoveResult CommandHandler::handleClick(
    ClientConnection& connection,
    const Command& command)
{

    if(!connection.hasPlayer())
    {
        return
        {
            false,
            "authentication_required"
        };
    }





    auto player =
        connection.getPlayer();





    if(!player->hasRoom())
    {
        return
        {
            false,
            "player_has_no_room"
        };
    }





    const auto& args =
        command.getArgs();





    if(args.size()!=2)
    {
        return
        {
            false,
            "invalid_click_arguments"
        };
    }





    int row;
    int col;



    try
    {

        row =
            std::stoi(args[0]);


        col =
            std::stoi(args[1]);

    }
    catch(...)
    {

        return
        {
            false,
            "invalid_position"
        };

    }






    Position position(
        row,
        col);





    ClickCommand click(
        position);






    auto room =
        m_roomManager.getRoom(
            player->getRoomId());






    if(!room)
    {
        return
        {
            false,
            "room_not_found"
        };
    }







    return
        room->getSession()
            .handleClick(
                *player,
                click);

}









//================================================
// CREATE ROOM
//================================================

MoveResult CommandHandler::handleCreateRoom(
    ClientConnection& connection,
    const Command& command)
{

    if(!connection.hasPlayer())
    {
        return
        {
            false,
            "authentication_required"
        };
    }




    const auto& args =
        command.getArgs();




    if(args.size()!=1)
    {
        return
        {
            false,
            "invalid_room_arguments"
        };
    }






    if(
        m_roomManager.createRoom(
            args[0]))
    {

        return
        {
            true,
            "room_created"
        };

    }






    return
    {
        false,
        "room_creation_failed"
    };

}









//================================================
// JOIN ROOM
//================================================

MoveResult CommandHandler::handleJoinRoom(
    ClientConnection& connection,
    const Command& command)
{

    if(!connection.hasPlayer())
    {
        return
        {
            false,
            "authentication_required"
        };
    }





    auto player =
        connection.getPlayer();






    const auto& args =
        command.getArgs();





    if(args.size()!=1)
    {
        return
        {
            false,
            "invalid_room_arguments"
        };
    }





    auto room =
        m_roomManager.getRoom(
            args[0]);





    if(!room)
    {
        return
        {
            false,
            "room_not_found"
        };
    }





    if(!room->canJoin())
    {
        return
        {
            false,
            "room_full"
        };
    }






    if(
        room->getSession()
            .addPlayer(
                player))
    {

        return
        {
            true,
            "joined_room"
        };

    }






    return
    {
        false,
        "join_failed"
    };

}