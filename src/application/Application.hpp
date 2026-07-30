#pragma once

#include <cstdint>
#include <memory>
#include <string>
#include "DatabaseConfiguration.hpp"
#include "MoveResult.hpp"

class EventBus;
class IDatabase;
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
 * @brief Represents the main application composition root responsible for instantiating services, managing dependencies, controlling component lifecycles, and driving server execution.
 */
class Application
{
public:
    /**
     * @brief Constructs an Application instance with specified port, database file path, and bind address.
     * @param port The network port number to listen on.
     * @param databasePath The file path to the database file.
     * @param bindAddress The network address on which the server listens.
     */
    explicit Application(
        std::uint16_t port = 8080,
        std::string databasePath = "kungfu_chess.db",
        std::string bindAddress = "127.0.0.1");

    explicit Application(
        std::uint16_t port,
        DatabaseConfiguration databaseConfiguration,
        std::string bindAddress = "127.0.0.1");

    /**
     * @brief Destroys the Application instance and releases owned resources.
     */
    ~Application();

    Application(const Application&) = delete;
    Application& operator=(const Application&) = delete;

public:
    /**
     * @brief Initializes application dependencies and starts network server execution.
     */
    void start();

    /**
     * @brief Stops network server execution and shuts down active loops.
     */
    void stop();

    /**
     * @brief Direct command execution method intended for internal testing.
     * @param message Raw string payload representing the incoming command.
     * @return Result status of the executed action.
     */
    MoveResult sendCommand(const std::string& message);

    /**
     * @brief Retrieves a reference to the active Server component.
     * @return Reference to the internal Server instance.
     */
    Server& getServer();

    /**
     * @brief Retrieves a reference to the active RoomManager component.
     * @return Reference to the internal RoomManager instance.
     */
    RoomManager& getRoomManager();

private:
    void initialize();

    std::unique_ptr<EventBus> m_eventBus;
    std::unique_ptr<IDatabase> m_database;
    std::unique_ptr<DatabaseInitializer> m_databaseInitializer;
    std::unique_ptr<UserRepository> m_userRepository;
    std::unique_ptr<PlayerSessionManager> m_playerSessionManager;
    std::unique_ptr<AuthService> m_authService;
    std::unique_ptr<RoomFactory> m_roomFactory;
    std::unique_ptr<RoomManager> m_roomManager;
    std::unique_ptr<PlayerLifecycleService> m_playerLifecycleService;
    std::unique_ptr<CommandHandler> m_commandHandler;
    std::unique_ptr<Server> m_server;
    std::unique_ptr<AuthoritativeGameLoop> m_gameLoop;
    std::uint16_t m_port;
    DatabaseConfiguration m_databaseConfiguration;
    std::string m_bindAddress;
    bool m_running{false};
};
