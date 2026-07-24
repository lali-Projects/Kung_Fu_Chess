#include "CommandHandler.hpp"


#include "ClickCommand.hpp"

#include "SessionManager.hpp"
#include "GameSession.hpp"
#include "PlayerSession.hpp"



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
        CommandHandler is only a dispatcher.

        Responsibility:

            ClickCommand
                  |
                  v
            GameSession


        It does not:
            - parse input
            - validate chess rules
            - modify board
            - execute movement
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