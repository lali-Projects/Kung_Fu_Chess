#pragma once


#include <memory>
#include <string>


#include "MoveResult.hpp"



class EventBus;

class IDatabase;
class SQLiteDatabase;
class DatabaseInitializer;

class UserRepository;
class AuthService;

class RoomFactory;
class RoomManager;

class CommandHandler;
class Server;




/**
 * @brief Main application composition root.
 *
 * Responsible for:
 *
 *  - Creating application services.
 *  - Connecting dependencies.
 *  - Managing lifetime of components.
 *  - Starting and stopping server.
 *
 *
 * Does NOT know:
 *
 *  - Game rules.
 *  - Board implementation.
 *  - Network protocol details.
 *  - Database SQL logic.
 */
class Application
{

public:


    Application();


    ~Application();



    Application(
        const Application&) = delete;



    Application& operator=(
        const Application&) = delete;




public:


    /**
     * @brief Starts application.
     */
    void start();




    /**
     * @brief Stops application.
     */
    void stop();





    /**
     * @brief Sends command directly for testing.
     */
    MoveResult sendCommand(
        const std::string& message);





    /**
     * @brief Access server.
     */
    Server& getServer();




    /**
     * @brief Access room manager.
     */
    RoomManager& getRoomManager();





private:


    /**
     * @brief Builds dependency graph.
     */
    void initialize();





private:


    //---------------------------------
    // Events
    //---------------------------------

    std::unique_ptr<EventBus>
        m_eventBus;





    //---------------------------------
    // Database
    //---------------------------------

    std::unique_ptr<IDatabase>
        m_database;



    std::unique_ptr<DatabaseInitializer>
        m_databaseInitializer;





    //---------------------------------
    // Authentication
    //---------------------------------

    std::unique_ptr<UserRepository>
        m_userRepository;



    std::unique_ptr<AuthService>
        m_authService;





    //---------------------------------
    // Rooms
    //---------------------------------

    std::unique_ptr<RoomFactory>
        m_roomFactory;



    std::unique_ptr<RoomManager>
        m_roomManager;





    //---------------------------------
    // Commands
    //---------------------------------

    std::unique_ptr<CommandHandler>
        m_commandHandler;





    //---------------------------------
    // Server
    //---------------------------------

    std::unique_ptr<Server>
        m_server;





private:


    bool m_running{
        false
    };

};