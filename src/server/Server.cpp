#include "Server.hpp"

#include <iostream>



//================================================
// Constructor
//================================================

Server::Server(
    CommandParser& commandParser,
    CommandHandler& commandHandler)
:
m_commandParser(commandParser),
m_commandHandler(commandHandler)
{
}



//================================================
// Destructor
//================================================

Server::~Server()
{
    stop();
}



//================================================
// Start Server
//================================================

void Server::start()
{
    if(m_running)
    {
        return;
    }


    m_running = true;


    std::cout
        << "Server started"
        << std::endl;
}



//================================================
// Stop Server
//================================================

void Server::stop()
{
    if(!m_running)
    {
        return;
    }


    m_running = false;


    std::cout
        << "Server stopped"
        << std::endl;
}



//================================================
// Is Running
//================================================

bool Server::isRunning() const
{
    return m_running;
}



//================================================
// Handle Client Message
//================================================

MoveResult Server::handle(
    const std::string& message)
{
    if(!m_running)
    {
        return
        {
            false,
            "server_not_running"
        };
    }



    /*
        Raw client message:

        Example:

        CLICK 6 0


        Server does not understand
        the command.

        It only performs:

        Text
          |
          v
        CommandParser
          |
          v
        Command
          |
          v
        CommandHandler
    */



    auto command =
        m_commandParser.parse(
            message);



    if(!command.has_value())
    {
        return
        {
            false,
            "invalid_command"
        };
    }



    return
        m_commandHandler.handle(
            command.value());
}