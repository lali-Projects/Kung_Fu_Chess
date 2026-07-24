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



    return m_commandHandler.handle(
        m_player,
        command.value());

}