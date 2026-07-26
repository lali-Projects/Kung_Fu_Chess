#include "ConnectionHandler.hpp"


#include "CommandHandler.hpp"
#include "PlayerSession.hpp"





//================================================
// Constructor
//================================================

ConnectionHandler::ConnectionHandler(
    CommandHandler& commandHandler)
:
m_commandHandler(commandHandler)
{
}






//================================================
// Receive
//================================================

MoveResult ConnectionHandler::receive(
    const std::string& message,
    PlayerSession* player)
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
            player,
            *command);

}