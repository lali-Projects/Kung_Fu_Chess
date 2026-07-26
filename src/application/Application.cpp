#include "Application.hpp"


#include <iostream>
#include <stdexcept>


#include "EventBus.hpp"

#include "RoomFactory.hpp"
#include "RoomManager.hpp"

#include "CommandHandler.hpp"

#include "Server.hpp"

#include "WebSocketServer.hpp"





Application::Application()
{
    initialize();
}



Application::~Application()
{
    stop();
}





void Application::initialize()
{

    m_eventBus =
        std::make_unique<EventBus>();



    m_roomFactory =
        std::make_unique<RoomFactory>(
            *m_eventBus);



    m_roomManager =
        std::make_unique<RoomManager>(
            *m_roomFactory);



    if(!m_roomManager->createRoom(
            "room_001"))
    {
        throw std::runtime_error(
            "Failed creating room");
    }




    m_commandHandler =
        std::make_unique<CommandHandler>(
            *m_roomManager);




    auto network =
        std::make_unique<WebSocketServer>(
            8080);



    m_server =
        std::make_unique<Server>(
            *m_commandHandler,
            *m_eventBus,
            std::move(network));

}






void Application::start()
{

    if(m_running)
        return;


    m_server->start();


    m_running = true;


    std::cout
        << "[APPLICATION] Started\n";

}






void Application::stop()
{

    if(!m_running)
        return;


    if(m_server)
        m_server->stop();


    m_running=false;


    std::cout
        << "[APPLICATION] Stopped\n";

}






MoveResult Application::sendCommand(
    const std::string& message)
{

    return
        m_server->simulateClientCommand(
            message);

}





Server&
Application::getServer()
{
    if(!m_server)
        throw std::runtime_error(
            "Server missing");


    return *m_server;
}





RoomManager&
Application::getRoomManager()
{

    if(!m_roomManager)
        throw std::runtime_error(
            "RoomManager missing");


    return *m_roomManager;

}