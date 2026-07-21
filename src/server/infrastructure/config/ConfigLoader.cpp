#include "ConfigLoader.hpp"

#include <fstream>
#include <stdexcept>
#include <string>

using json = nlohmann::json;

//======================================================
// Public API
//======================================================

ServerConfig ConfigLoader::load(const std::filesystem::path& filePath)
{
    std::ifstream file(filePath);
    if (!file.is_open())
    {
        throw std::runtime_error("Cannot open configuration file: " + filePath.string());
    }

    json document;
    try
    {
        /* Parsing happens exactly once here */
        file >> document;
    }
    catch (const json::exception& e)
    {
        throw std::runtime_error(std::string("Invalid JSON configuration: ") + e.what());
    }

    ServerConfig config;
    config.server = parseServer(document);
    config.timing = parseTiming(document);
    config.database = parseDatabase(document);
    config.logging = parseLogging(document);

    validate(config);

    return config;
}

//======================================================
// Parsing Helpers
//======================================================

ServerSettings ConfigLoader::parseServer(const json& document)
{
    if (!document.contains("server"))
    {
        throw std::runtime_error("Missing server section");
    }

    const auto& server = document.at("server");

    ServerSettings settings;
    settings.port = server.at("port").get<std::uint16_t>();
    settings.maxClients = server.at("maxClients").get<std::size_t>();
    settings.maxSessions = server.at("maxSessions").get<std::size_t>();

    return settings;
}

TimingSettings ConfigLoader::parseTiming(const json& document)
{
    if (!document.contains("timing"))
    {
        throw std::runtime_error("Missing timing section");
    }

    const auto& timing = document.at("timing");

    TimingSettings settings;
    settings.tickRateHz = timing.at("tickRateHz").get<std::uint32_t>();
    settings.broadcastRateHz = timing.at("broadcastRateHz").get<std::uint32_t>();
    settings.heartbeatTimeoutSeconds = timing.at("heartbeatTimeoutSeconds").get<std::uint32_t>();
    settings.reconnectTimeoutSeconds = timing.at("reconnectTimeoutSeconds").get<std::uint32_t>();
    settings.matchmakingTimeoutSeconds = timing.at("matchmakingTimeoutSeconds").get<std::uint32_t>();

    return settings;
}

DatabaseSettings ConfigLoader::parseDatabase(const json& document)
{
    if (!document.contains("database"))
    {
        throw std::runtime_error("Missing database section");
    }

    const auto& database = document.at("database");

    DatabaseSettings settings;
    settings.databasePath = database.at("path").get<std::string>();

    return settings;
}

LoggingSettings ConfigLoader::parseLogging(const json& document)
{
    if (!document.contains("logging"))
    {
        throw std::runtime_error("Missing logging section");
    }

    const auto& logging = document.at("logging");

    LoggingSettings settings;
    settings.logDirectory = logging.at("directory").get<std::string>();

    return settings;
}

//======================================================
// Validation Helpers
//======================================================

void ConfigLoader::validate(const ServerConfig& config)
{
    validateServer(config.server);
    validateTiming(config.timing);
    validateDatabase(config.database);
    validateLogging(config.logging);
}

void ConfigLoader::validateServer(const ServerSettings& settings)
{
    if (settings.port == 0)
    {
        throw std::runtime_error("Server port cannot be zero");
    }

    if (settings.maxClients == 0)
    {
        throw std::runtime_error("maxClients must be greater than zero");
    }

    if (settings.maxSessions == 0)
    {
        throw std::runtime_error("maxSessions must be greater than zero");
    }
}

void ConfigLoader::validateTiming(const TimingSettings& settings)
{
    if (settings.tickRateHz == 0)
    {
        throw std::runtime_error("Tick rate cannot be zero");
    }

    if (settings.broadcastRateHz == 0)
    {
        throw std::runtime_error("Broadcast rate cannot be zero");
    }

    if (settings.broadcastRateHz > settings.tickRateHz)
    {
        throw std::runtime_error("Broadcast rate cannot exceed tick rate");
    }
}

void ConfigLoader::validateDatabase(const DatabaseSettings& settings)
{
    if (settings.databasePath.empty())
    {
        throw std::runtime_error("Database path is empty");
    }
}

void ConfigLoader::validateLogging(const LoggingSettings& settings)
{
    if (settings.logDirectory.empty())
    {
        throw std::runtime_error("Log directory is empty");
    }
}