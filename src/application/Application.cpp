#include "Application.hpp"
#include <iostream>
#include <stdexcept>
#include <utility>

#include "EventBus.hpp"
#include "IDatabase.hpp"
#include "SQLiteDatabase.hpp"
#ifdef KFC_HAS_POSTGRESQL
#include "PostgreSQLDatabase.hpp"
#endif
#include "DatabaseInitializer.hpp"
#include "UserRepository.hpp"
#include "AuthService.hpp"
#include "PlayerSessionManager.hpp"
#include "PlayerLifecycleService.hpp"
#include "AuthoritativeGameLoop.hpp"
#include "RoomFactory.hpp"
#include "RoomManager.hpp"
#include "CommandHandler.hpp"
#include "Server.hpp"
#include "WebSocketServer.hpp"

Application::Application(
    std::uint16_t port,
    std::string databasePath,
    std::string bindAddress)
    : Application(
          port,
          DatabaseConfiguration{
              DatabaseType::SQLITE,
              std::move(databasePath),
              {}},
          std::move(bindAddress))
{
}

Application::Application(
    std::uint16_t port,
    DatabaseConfiguration databaseConfiguration,
    std::string bindAddress)
    : m_port(port),
      m_databaseConfiguration(std::move(databaseConfiguration)),
      m_bindAddress(std::move(bindAddress))
{
    if (m_port == 0)
    {
        throw std::invalid_argument("Server port cannot be zero");
    }

    if (m_databaseConfiguration.type == DatabaseType::SQLITE &&
        m_databaseConfiguration.sqlitePath.empty())
    {
        throw std::invalid_argument("Database path cannot be empty");
    }

    if (m_databaseConfiguration.type == DatabaseType::POSTGRESQL)
    {
        const auto& postgresql = m_databaseConfiguration.postgresql;

        if (postgresql.host.empty() ||
            postgresql.port == 0 ||
            postgresql.database.empty() ||
            postgresql.user.empty() ||
            postgresql.password.empty())
        {
            throw std::invalid_argument(
                "PostgreSQL configuration is incomplete");
        }
    }

    if (m_bindAddress.empty())
    {
        throw std::invalid_argument("Server bind address cannot be empty");
    }

    initialize();
}

Application::~Application()
{
    stop();
}

void Application::initialize()
{
    m_eventBus = std::make_unique<EventBus>();

    DatabaseDialect databaseDialect = DatabaseDialect::SQLITE;

    if (m_databaseConfiguration.type == DatabaseType::SQLITE)
    {
        m_database = std::make_unique<SQLiteDatabase>(
            m_databaseConfiguration.sqlitePath);
    }
    else
    {
#ifdef KFC_HAS_POSTGRESQL
        m_database = std::make_unique<PostgreSQLDatabase>(
            m_databaseConfiguration.postgresql);
        databaseDialect = DatabaseDialect::POSTGRESQL;
#else
        throw std::runtime_error(
            "PostgreSQL backend selected, but this server was built "
            "without PostgreSQL support");
#endif
    }

    if (!m_database->open())
    {
        throw std::runtime_error("Database open failed: " + m_database->getLastError());
    }

    m_databaseInitializer = std::make_unique<DatabaseInitializer>(
        *m_database,
        databaseDialect);

    if (!m_databaseInitializer->initialize())
    {
        throw std::runtime_error(
            "Database initialization failed: " +
            m_database->getLastError());
    }

    std::cout << "[APPLICATION] Database initialized\n";

    m_userRepository = std::make_unique<UserRepository>(*m_database);
    m_playerSessionManager = std::make_unique<PlayerSessionManager>();

    m_authService = std::make_unique<AuthService>(*m_userRepository, *m_playerSessionManager);

    m_roomFactory = std::make_unique<RoomFactory>(*m_eventBus);
    m_roomManager = std::make_unique<RoomManager>(*m_roomFactory);
    m_playerLifecycleService = std::make_unique<PlayerLifecycleService>(*m_roomManager, *m_playerSessionManager);

    if (!m_roomManager->createRoom("room_001"))
    {
        throw std::runtime_error("Default room creation failed");
    }

    m_commandHandler = std::make_unique<CommandHandler>(*m_roomManager, *m_authService, *m_playerLifecycleService);

    auto network = std::make_unique<WebSocketServer>(m_port, m_bindAddress);

    m_server = std::make_unique<Server>(*m_commandHandler, *m_eventBus, *m_playerLifecycleService, std::move(network));
    m_gameLoop = std::make_unique<AuthoritativeGameLoop>(*m_roomManager);
}

void Application::start()
{
    if (m_running)
    {
        return;
    }

    if (!m_server)
    {
        throw std::runtime_error("Cannot start: server missing");
    }

    m_server->start();

    try
    {
        m_gameLoop->start();
    }
    catch (...)
    {
        m_server->stop();
        throw;
    }

    m_running = true;

    std::cout << "[APPLICATION] Started\n";
}

void Application::stop()
{
    if (!m_running)
    {
        return;
    }

    if (m_gameLoop)
    {
        m_gameLoop->stop();
        std::cout << "[APPLICATION] Game loop stopped\n";
    }

    if (m_server)
    {
        m_server->stop();
    }

    if (m_database && m_database->isOpen())
    {
        m_database->close();

        if (m_database->isOpen())
        {
            std::cerr << "[APPLICATION] Database close failed\n";
        }
        else
        {
            std::cout << "[APPLICATION] Database closed\n";
        }
    }

    m_running = false;

    std::cout << "[APPLICATION] Stopped\n";
}

MoveResult Application::sendCommand(const std::string& message)
{
    if (!m_server)
    {
        return {false, "server_missing"};
    }

    return m_server->simulateClientCommand(message);
}

Server& Application::getServer()
{
    if (!m_server)
    {
        throw std::runtime_error("Server missing");
    }

    return *m_server;
}

RoomManager& Application::getRoomManager()
{
    if (!m_roomManager)
    {
        throw std::runtime_error("RoomManager missing");
    }

    return *m_roomManager;
}
