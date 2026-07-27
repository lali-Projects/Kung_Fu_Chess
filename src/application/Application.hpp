#pragma once


#include <cstdint>
#include <memory>
#include <string>


#include "MoveResult.hpp"



class EventBus;

class IDatabase;
class SQLiteDatabase;
class DatabaseInitializer;

class UserRepository;
class AuthService;
class PlayerSessionManager;
class PlayerLifecycleService;
class AuthoritativeGameLoop;

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


    explicit Application(
        std::uint16_t port = 8080,
        std::string databasePath = "kungfu_chess.db");


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



    std::unique_ptr<PlayerSessionManager>
        m_playerSessionManager;



    std::unique_ptr<AuthService>
        m_authService;





    //---------------------------------
    // Rooms
    //---------------------------------

    std::unique_ptr<RoomFactory>
        m_roomFactory;



    std::unique_ptr<RoomManager>
        m_roomManager;



    std::unique_ptr<PlayerLifecycleService>
        m_playerLifecycleService;





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


    std::unique_ptr<AuthoritativeGameLoop>
        m_gameLoop;





private:


    std::uint16_t m_port;


    std::string m_databasePath;


    bool m_running{
        false
    };

};
