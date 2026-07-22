#include "ConnectionHandler.hpp"


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
// Receive Message
//================================================

MoveResult ConnectionHandler::receive(
    const std::string& message)
{
    /*
        Example input:

        "CLICK 3 4"


        Step 1:
        Parse message

        Step 2:
        Create ClickCommand

        Step 3:
        Execute command
    */


    auto command =
        m_parser.parse(message);



    /*
        Parsing failed.
    */

    if(!command.has_value())
    {
        return {
            false,
            "invalid_command"
        };
    }



    /*
        Forward command.

        ConnectionHandler does not know
        what CLICK means.
    */

    return m_commandHandler.handle(
        command.value());
}