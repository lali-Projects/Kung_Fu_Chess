#include <charconv>
#include <chrono>
#include <csignal>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <string>
#include <thread>

#include "Application.hpp"

#include <ixwebsocket/IXNetSystem.h>

namespace
{
    volatile std::sig_atomic_t stopRequested = 0;

    struct ServerConfiguration
    {
        std::uint16_t port{8080};
        std::string bindAddress{"127.0.0.1"};
        std::string databasePath{"kungfu_chess.db"};
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

    std::uint16_t parsePort(const std::string& value)
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
                "KFC_SERVER_PORT must be a number between 1 and 65535");
        }

        return static_cast<std::uint16_t>(parsedPort);
    }

    ServerConfiguration loadConfiguration()
    {
        ServerConfiguration configuration;

        const std::string port = readEnvironmentValue(
            "KFC_SERVER_PORT",
            "8080");
        configuration.port = parsePort(port);
        configuration.bindAddress = readEnvironmentValue(
            "KFC_SERVER_BIND_ADDRESS",
            "127.0.0.1");
        configuration.databasePath = readEnvironmentValue(
            "KFC_DATABASE_PATH",
            "kungfu_chess.db");

        if (configuration.bindAddress.empty())
        {
            throw std::invalid_argument(
                "KFC_SERVER_BIND_ADDRESS cannot be empty");
        }

        if (configuration.databasePath.empty())
        {
            throw std::invalid_argument(
                "KFC_DATABASE_PATH cannot be empty");
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
            configuration.databasePath,
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
