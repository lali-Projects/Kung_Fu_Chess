#include "ConnectionHandler.hpp"


#include "CommandHandler.hpp"
#include "ClientConnection.hpp"





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
    ClientConnection& connection)
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

    auto response =
        m_commandHandler.handle(
            connection,
            *command);


    return
    {
        response.success,
        response.reason
    };

}
