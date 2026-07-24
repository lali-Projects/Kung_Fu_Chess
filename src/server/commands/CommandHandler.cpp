#include "CommandHandler.hpp"



//================================================
// Constructor
//================================================

CommandHandler::CommandHandler(
    SessionManager& sessionManager)
    :
    m_sessionManager(sessionManager)
{
}



//================================================
// Handle Click
//================================================

MoveResult CommandHandler::handle(
    PlayerSession& player,
    const ClickCommand& command)
{

    /*
        CommandHandler only forwards
        the command to the active session.

        It does not know:
        - Board
        - Rules
        - GameEngine
    */


    if(!m_sessionManager.hasSession())
    {
        return
        {
            false,
            "no_active_session"
        };
    }



    GameSession& session =
        m_sessionManager.getSession();



    return session.handleClick(
        player,
        command);
}