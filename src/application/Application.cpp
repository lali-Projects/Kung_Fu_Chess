#include "Application.hpp"


#include <iostream>
#include <stdexcept>
#include <utility>


#include "EventBus.hpp"


// Database
#include "IDatabase.hpp"
#include "SQLiteDatabase.hpp"
#include "DatabaseInitializer.hpp"


// Authentication
#include "UserRepository.hpp"
#include "AuthService.hpp"
#include "PlayerSessionManager.hpp"
#include "PlayerLifecycleService.hpp"
#include "AuthoritativeGameLoop.hpp"


// Rooms
#include "RoomFactory.hpp"
#include "RoomManager.hpp"


// Commands
#include "CommandHandler.hpp"


// Server
#include "Server.hpp"
#include "WebSocketServer.hpp"






//================================================
// Constructor
//================================================

Application::Application(
    std::uint16_t port,
    std::string databasePath)
:
m_port(port),
m_databasePath(std::move(databasePath))
{
    if(m_port == 0)
    {
        throw std::invalid_argument(
            "Server port cannot be zero");
    }


    if(m_databasePath.empty())
    {
        throw std::invalid_argument(
            "Database path cannot be empty");
    }


    initialize();
}







//================================================
// Destructor
//================================================

Application::~Application()
{
    stop();
}







//================================================
// Initialize
//================================================

void Application::initialize()
{

    //---------------------------------
    // Event System
    //---------------------------------

    m_eventBus =
        std::make_unique<EventBus>();








    //---------------------------------
    // Database
    //---------------------------------

    m_database =
        std::make_unique<SQLiteDatabase>(
            m_databasePath);





    if(!m_database->open())
    {
        throw std::runtime_error(
            "Database open failed: "
            +
            m_database->getLastError());
    }








    //---------------------------------
    // Database Schema
    //---------------------------------

    m_databaseInitializer =
        std::make_unique<DatabaseInitializer>(
            *m_database);





    if(!m_databaseInitializer->initialize())
    {
        throw std::runtime_error(
            "Database initialization failed");
    }









    //---------------------------------
    // Authentication
    //---------------------------------

    m_userRepository =
        std::make_unique<UserRepository>(
            *m_database);


    m_playerSessionManager =
        std::make_unique<PlayerSessionManager>();





    m_authService =
        std::make_unique<AuthService>(
            *m_userRepository,
            *m_playerSessionManager);









    //---------------------------------
    // Rooms
    //---------------------------------

    m_roomFactory =
        std::make_unique<RoomFactory>(
            *m_eventBus);






    m_roomManager =
        std::make_unique<RoomManager>(
            *m_roomFactory);


    m_playerLifecycleService =
        std::make_unique<PlayerLifecycleService>(
            *m_roomManager,
            *m_playerSessionManager);









    //---------------------------------
    // Create Default Room
    //---------------------------------

    if(!m_roomManager->createRoom(
            "room_001"))
    {
        throw std::runtime_error(
            "Default room creation failed");
    }









    //---------------------------------
    // Commands
    //---------------------------------

    m_commandHandler =
        std::make_unique<CommandHandler>(
            *m_roomManager,
            *m_authService,
            *m_playerLifecycleService);









    //---------------------------------
    // Network
    //---------------------------------

    auto network =
        std::make_unique<WebSocketServer>(
            m_port);









    //---------------------------------
    // Server
    //---------------------------------

    m_server =
        std::make_unique<Server>(
            *m_commandHandler,
            *m_eventBus,
            *m_playerLifecycleService,
            std::move(network));


    m_gameLoop =
        std::make_unique<AuthoritativeGameLoop>(
            *m_roomManager);

}











//================================================
// Start
//================================================

void Application::start()
{

    if(m_running)
    {
        return;
    }






    if(!m_server)
    {
        throw std::runtime_error(
            "Cannot start: server missing");
    }







    m_server->start();


    try
    {
        m_gameLoop->start();
    }
    catch(...)
    {
        m_server->stop();
        throw;
    }




    m_running = true;





    std::cout
        << "[APPLICATION] Started\n";

}











//================================================
// Stop
//================================================

void Application::stop()
{

    if(!m_running)
    {
        return;
    }







    //---------------------------------
    // Stop Authoritative Game Loop
    //---------------------------------

    if(m_gameLoop)
    {
        m_gameLoop->stop();
    }


    //---------------------------------
    // Stop Network Server
    //---------------------------------

    if(m_server)
    {
        m_server->stop();
    }







    //---------------------------------
    // Close Database
    //---------------------------------

    if(m_database &&
       m_database->isOpen())
    {
        m_database->close();
    }







    m_running = false;






    std::cout
        << "[APPLICATION] Stopped\n";

}











//================================================
// Send Command
//================================================

MoveResult Application::sendCommand(
    const std::string& message)
{

    if(!m_server)
    {
        return
        {
            false,
            "server_missing"
        };
    }






    return
        m_server->simulateClientCommand(
            message);

}











//================================================
// Get Server
//================================================

Server&
Application::getServer()
{

    if(!m_server)
    {
        throw std::runtime_error(
            "Server missing");
    }






    return *m_server;

}











//================================================
// Get Room Manager
//================================================

RoomManager&
Application::getRoomManager()
{

    if(!m_roomManager)
    {
        throw std::runtime_error(
            "RoomManager missing");
    }






    return *m_roomManager;

}
