#include "Application.hpp"


#include <iostream>
#include <stdexcept>


#include "EventBus.hpp"


// Database
#include "IDatabase.hpp"
#include "SQLiteDatabase.hpp"
#include "DatabaseInitializer.hpp"


// Authentication
#include "UserRepository.hpp"
#include "AuthService.hpp"


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

Application::Application()
{
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
            "kungfu_chess.db");





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





    m_authService =
        std::make_unique<AuthService>(
            *m_userRepository);









    //---------------------------------
    // Rooms
    //---------------------------------

    m_roomFactory =
        std::make_unique<RoomFactory>(
            *m_eventBus);






    m_roomManager =
        std::make_unique<RoomManager>(
            *m_roomFactory);









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
            *m_authService);









    //---------------------------------
    // Network
    //---------------------------------

    auto network =
        std::make_unique<WebSocketServer>(
            8080);









    //---------------------------------
    // Server
    //---------------------------------

    m_server =
        std::make_unique<Server>(
            *m_commandHandler,
            *m_eventBus,
            std::move(network));

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