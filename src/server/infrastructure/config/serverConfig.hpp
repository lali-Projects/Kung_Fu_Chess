#pragma once

#include <cstdint>
#include <filesystem>
#include <string>

//======================================================
// Server
//======================================================

struct ServerSettings
{
    std::uint16_t port{};

    std::size_t maxClients{};

    std::size_t maxSessions{};
};

//======================================================
// Timing
//======================================================

struct TimingSettings
{
    std::uint32_t tickRateHz{};

    std::uint32_t broadcastRateHz{};

    std::uint32_t heartbeatTimeoutSeconds{};

    std::uint32_t reconnectTimeoutSeconds{};

    std::uint32_t matchmakingTimeoutSeconds{};
};

//======================================================
// Database
//======================================================

struct DatabaseSettings
{
    std::filesystem::path databasePath;
};

//======================================================
// Logging
//======================================================

struct LoggingSettings
{
    std::filesystem::path logDirectory;
};

//======================================================
// Root Configuration Object
//======================================================

struct ServerConfig
{
    ServerSettings server;

    TimingSettings timing;

    DatabaseSettings database;

    LoggingSettings logging;
};