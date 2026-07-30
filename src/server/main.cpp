#include <charconv>
#include <chrono>
#include <csignal>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <string>
#include <thread>
#include <utility>

#include "Application.hpp"
#include "DatabaseConfiguration.hpp"

#include <ixwebsocket/IXNetSystem.h>

namespace
{
    volatile std::sig_atomic_t stopRequested = 0;

    struct ServerConfiguration
    {
        std::uint16_t port{8080};
        std::string bindAddress{"127.0.0.1"};
        DatabaseConfiguration database;
    };

    void handleSignal(int)
    {
        stopRequested = 1;
    }

    std::string readEnvironmentValue(const char* name, const char* defaultValue)
    {
        const char* value = std::getenv(name);
        return value ? value : defaultValue;
    }

    std::uint16_t parsePort(
        const std::string& value,
        const char* variableName)
    {
        std::uint32_t parsedPort = 0;
        const auto result = std::from_chars(
            value.data(),
            value.data() + value.size(),
            parsedPort);

        if (value.empty() ||
            result.ec != std::errc{} ||
            result.ptr != value.data() + value.size() ||
            parsedPort == 0 ||
            parsedPort > 65535)
        {
            throw std::invalid_argument(
                std::string(variableName) +
                " must be a number between 1 and 65535");
        }

        return static_cast<std::uint16_t>(parsedPort);
    }

    ServerConfiguration loadConfiguration()
    {
        ServerConfiguration configuration;

        const std::string port = readEnvironmentValue(
            "KFC_SERVER_PORT",
            "8080");
        configuration.port = parsePort(port, "KFC_SERVER_PORT");
        configuration.bindAddress = readEnvironmentValue(
            "KFC_SERVER_BIND_ADDRESS",
            "127.0.0.1");

        if (configuration.bindAddress.empty())
        {
            throw std::invalid_argument(
                "KFC_SERVER_BIND_ADDRESS cannot be empty");
        }

        const std::string databaseType = readEnvironmentValue(
            "KFC_DATABASE_TYPE",
            "sqlite");

        if (databaseType == "sqlite")
        {
            configuration.database.type = DatabaseType::SQLITE;
            configuration.database.sqlitePath = readEnvironmentValue(
                "KFC_DATABASE_PATH",
                "kungfu_chess.db");

            if (configuration.database.sqlitePath.empty())
            {
                throw std::invalid_argument(
                    "KFC_DATABASE_PATH cannot be empty");
            }
        }
        else if (databaseType == "postgresql")
        {
            configuration.database.type = DatabaseType::POSTGRESQL;
            auto& postgresql = configuration.database.postgresql;
            postgresql.host = readEnvironmentValue(
                "KFC_POSTGRES_HOST",
                "127.0.0.1");
            postgresql.port = parsePort(
                readEnvironmentValue("KFC_POSTGRES_PORT", "5432"),
                "KFC_POSTGRES_PORT");
            postgresql.database = readEnvironmentValue(
                "KFC_POSTGRES_DATABASE",
                "kungfu_chess");
            postgresql.user = readEnvironmentValue(
                "KFC_POSTGRES_USER",
                "kfc");
            postgresql.password = readEnvironmentValue(
                "KFC_POSTGRES_PASSWORD",
                "");

            if (postgresql.host.empty())
            {
                throw std::invalid_argument(
                    "KFC_POSTGRES_HOST cannot be empty");
            }

            if (postgresql.database.empty())
            {
                throw std::invalid_argument(
                    "KFC_POSTGRES_DATABASE cannot be empty");
            }

            if (postgresql.user.empty())
            {
                throw std::invalid_argument(
                    "KFC_POSTGRES_USER cannot be empty");
            }

            if (postgresql.password.empty())
            {
                throw std::invalid_argument(
                    "KFC_POSTGRES_PASSWORD is required when "
                    "KFC_DATABASE_TYPE=postgresql");
            }
        }
        else
        {
            throw std::invalid_argument(
                "KFC_DATABASE_TYPE must be either sqlite or postgresql");
        }

        return configuration;
    }
}

int main()
{
    ServerConfiguration configuration;

    try
    {
        configuration = loadConfiguration();
    }
    catch (const std::exception& exception)
    {
        std::cerr << "[SERVER CONFIG ERROR] " << exception.what() << '\n';
        return 1;
    }

    if (std::signal(SIGINT, handleSignal) == SIG_ERR ||
        std::signal(SIGTERM, handleSignal) == SIG_ERR)
    {
        std::cerr << "[SERVER ERROR] Failed to install signal handlers\n";
        return 1;
    }

    ix::initNetSystem();

    try
    {
        Application app(
            configuration.port,
            std::move(configuration.database),
            configuration.bindAddress);

        app.start();

        std::cout << "[SERVER MAIN] Running\n";

        while (!stopRequested)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }

        std::cout << "[SERVER MAIN] Shutdown signal received\n";
        app.stop();
    }
    catch (const std::exception& exception)
    {
        std::cerr << "[SERVER ERROR] " << exception.what() << '\n';
        ix::uninitNetSystem();
        return 1;
    }

    ix::uninitNetSystem();
    std::cout << "[SERVER MAIN] Network uninitialized\n";

    return 0;
}
