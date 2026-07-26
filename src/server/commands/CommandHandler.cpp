#include "CommandHandler.hpp"


#include <iostream>


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


std::cout
    << "[COMMAND HANDLER] handling command"
    << std::endl;
    if(!m_sessionManager.hasSession())
    {
        return
        {
            false,
            "no_active_session"
        };
    }



    std::cout
        << "[COMMAND] Forwarding to session"
        << std::endl;



    GameSession& session =
        m_sessionManager.getSession();



    return session.handleClick(
        player,
        command);

}