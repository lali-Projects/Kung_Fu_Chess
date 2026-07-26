#include "ConnectionHandler.hpp"


#include "CommandHandler.hpp"
#include "PlayerSession.hpp"




//================================================
// Constructor
//================================================

ConnectionHandler::ConnectionHandler(
    CommandHandler& commandHandler,
    PlayerSession& player)
:
m_commandHandler(commandHandler),
m_player(player)
{
}






//================================================
// Receive
//================================================

MoveResult ConnectionHandler::receive(
    const std::string& message)
{

    //---------------------------------
    // Validate message
    //---------------------------------

    if(message.empty())
    {
        return
        {
            false,
            "empty_message"
        };
    }





    //---------------------------------
    // Validate player connection
    //---------------------------------

    if(
        m_player.getState()
        ==
        PlayerSession::ConnectionState::DISCONNECTED)
    {
        return
        {
            false,
            "player_disconnected"
        };
    }





    //---------------------------------
    // Parse command
    //---------------------------------

    auto command =
        m_parser.parse(message);



    if(!command.has_value())
    {
        return
        {
            false,
            "invalid_command"
        };
    }





    //---------------------------------
    // Forward command
    //---------------------------------

    return
        m_commandHandler.handle(
            m_player,
            *command);

}