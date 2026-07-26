#include "CommandHandler.hpp"


#include "Command.hpp"
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
// Handle
//================================================

MoveResult CommandHandler::handle(
    PlayerSession* player,
    const Command& command)
{

    switch(command.getType())
    {


        case CommandType::CLICK:
        {
            return handleClick(
                player,
                command);
        }



        case CommandType::LOGIN:
        {
            return handleLogin(
                command);
        }



        case CommandType::REGISTER:
        {
            return handleRegister(
                command);
        }



        case CommandType::CREATE_ROOM:
        {
            return handleCreateRoom(
                command);
        }



        case CommandType::JOIN_ROOM:
        {
            return handleJoinRoom(
                player,
                command);
        }



        default:
        {
            return
            {
                false,
                "unknown_command"
            };
        }

    }

}







//================================================
// CLICK
//================================================

MoveResult CommandHandler::handleClick(
    PlayerSession* player,
    const Command& command)
{

    if(!player)
    {
        return
        {
            false,
            "authentication_required"
        };
    }



    if(!player->isConnected())
    {
        return
        {
            false,
            "player_disconnected"
        };
    }



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



    if(args.size() != 2)
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
        row = std::stoi(args[0]);
        col = std::stoi(args[1]);
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
// LOGIN
//================================================

MoveResult CommandHandler::handleLogin(
    const Command& command)
{

    const auto& args =
        command.getArgs();



    if(args.size() != 2)
    {
        return
        {
            false,
            "invalid_login_arguments"
        };
    }



    return
        m_authService.login(
            args[0],
            args[1]);

}








//================================================
// REGISTER
//================================================

MoveResult CommandHandler::handleRegister(
    const Command& command)
{

    const auto& args =
        command.getArgs();



    if(args.size() != 2)
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
// CREATE ROOM
//================================================

MoveResult CommandHandler::handleCreateRoom(
    const Command& command)
{

    const auto& args =
        command.getArgs();



    if(args.size() != 1)
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
    PlayerSession* player,
    const Command& command)
{

    if(!player)
    {
        return
        {
            false,
            "authentication_required"
        };
    }



    const auto& args =
        command.getArgs();



    if(args.size() != 1)
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
                std::shared_ptr<PlayerSession>(
                    player,
                    [](PlayerSession*){})))
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