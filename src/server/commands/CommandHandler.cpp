#include "CommandHandler.hpp"


#include "ClientConnection.hpp"
#include "Command.hpp"

#include "Position.hpp"
#include "ClickCommand.hpp"

#include "RoomManager.hpp"
#include "Room.hpp"

#include "GameSession.hpp"

#include "PlayerSession.hpp"
#include "PlayerLifecycleService.hpp"

#include "AuthService.hpp"


#include <string>


namespace
{
    std::string commandName(
        CommandType type)
    {
        switch(type)
        {
            case CommandType::CLICK: return "CLICK";
            case CommandType::LOGIN: return "LOGIN";
            case CommandType::REGISTER: return "REGISTER";
            case CommandType::CREATE_ROOM: return "CREATE_ROOM";
            case CommandType::JOIN_ROOM: return "JOIN_ROOM";
            case CommandType::LEAVE_ROOM: return "LEAVE_ROOM";
            case CommandType::LOGOUT: return "LOGOUT";
            default: return "UNKNOWN";
        }
    }
}




//================================================
// Constructor
//================================================

CommandHandler::CommandHandler(
    RoomManager& roomManager,
    AuthService& authService,
    PlayerLifecycleService& playerLifecycle)
:
m_roomManager(roomManager),
m_authService(authService),
m_playerLifecycle(playerLifecycle)
{
}






//================================================
// Main Dispatcher
//================================================

CommandResponse CommandHandler::handle(
    ClientConnection& connection,
    const Command& command)
{
    CommandResponse response;
    response.command =
        commandName(command.getType());


    auto playerBefore =
        connection.getPlayer();


    if(playerBefore)
    {
        if(playerBefore->hasUser())
        {
            response.userId = playerBefore->getUserId();
            response.username = playerBefore->getUsername();
        }


        response.sessionId =
            playerBefore->getSessionId();
    }


    MoveResult result;

    switch(command.getType())
    {


        case CommandType::REGISTER:

            result = handleRegister(
                command);
            break;



        case CommandType::LOGIN:

            result = handleLogin(
                connection,
                command);
            break;



        case CommandType::CLICK:

            result = handleClick(
                connection,
                command);
            break;



        case CommandType::CREATE_ROOM:

            result = handleCreateRoom(
                connection,
                command);
            break;



        case CommandType::JOIN_ROOM:

            result = handleJoinRoom(
                connection,
                command);
            break;


        case CommandType::LEAVE_ROOM:

            result = handleLeaveRoom(
                connection);
            break;


        case CommandType::LOGOUT:

            result = handleLogout(
                connection);
            break;



        default:

            result = {false, "unknown_command"};
            break;
    }


    response.success = result.success;
    response.reason = result.reason;


    auto playerAfter =
        connection.getPlayer();


    if(playerAfter)
    {
        if(playerAfter->hasUser())
        {
            response.userId = playerAfter->getUserId();
            response.username = playerAfter->getUsername();
        }


        response.sessionId =
            playerAfter->getSessionId();


        if(playerAfter->hasRoom())
        {
            response.roomId = playerAfter->getRoomId();


            if(playerAfter->getSide() != Side::NONE)
            {
                response.side = playerAfter->getSide();
            }
        }


        if(command.getType() == CommandType::CLICK &&
           playerAfter->hasRoom())
        {
            auto room =
                m_roomManager.getRoom(
                    playerAfter->getRoomId());


            if(room)
            {
                response.selectedPosition =
                    room->getSession()
                        .getSelectedPosition(
                            *playerAfter);
            }
        }
    }


    if(command.getType() == CommandType::CREATE_ROOM &&
       result.success &&
       !command.getArgs().empty())
    {
        response.roomId =
            command.getArgs().front();
    }


    return response;

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
    if(connection.hasPlayer())
    {
        return
        {
            false,
            "already_authenticated"
        };
    }


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



    if(!connection.attachPlayer(player))
    {
        m_playerLifecycle.disconnect(player);
        return {false, "session_attach_failed"};
    }



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

    auto player =
        connection.getPlayer();


    if(!player ||
       !player->isAuthenticated())
    {
        return
        {
            false,
            "authentication_required"
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

    auto player =
        connection.getPlayer();


    if(!player ||
       !player->isAuthenticated())
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
    const auto& args =
        command.getArgs();


    if(args.size()!=1)
    {
        return {false, "invalid_room_arguments"};
    }


    auto player =
        connection.getPlayer();


    if(!player ||
       !player->isAuthenticated())
    {
        return {false, "authentication_required"};
    }


    return m_playerLifecycle.joinRoom(
        player,
        args[0]);

}


MoveResult CommandHandler::handleLeaveRoom(
    ClientConnection& connection)
{
    auto player =
        connection.getPlayer();


    if(!player ||
       !player->isAuthenticated())
    {
        return {false, "authentication_required"};
    }


    return m_playerLifecycle.leaveRoom(player);
}


MoveResult CommandHandler::handleLogout(
    ClientConnection& connection)
{
    auto player =
        connection.getPlayer();


    if(!player ||
       !player->isAuthenticated())
    {
        return {false, "authentication_required"};
    }


    MoveResult result =
        m_playerLifecycle.logout(player);


    if(result.success)
    {
        connection.clearPlayer();
    }


    return result;
}
