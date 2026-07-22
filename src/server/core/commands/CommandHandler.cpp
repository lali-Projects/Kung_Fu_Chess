#include "CommandHandler.hpp"


//================================================
// Constructor
//================================================

CommandHandler::CommandHandler(
    GameSession& session)
    :
    m_session(session)
{
}



//================================================
// Handle Click
//================================================

MoveResult CommandHandler::handle(
    const ClickCommand& command)
{
    /*
        CommandHandler does not interpret
        the command.

        It only forwards the data.

        Decision between:
          - move
          - jump

        remains inside GameController.
    */

    return m_session.handleClick(
        command.getPosition());
}