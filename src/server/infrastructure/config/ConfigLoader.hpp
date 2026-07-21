#pragma once

#include <filesystem>
#include <nlohmann/json.hpp>
#include "ServerConfig.hpp"

using Json = nlohmann::json;

/**
 * @brief Loads and validates server configuration.
 *
 * Responsibilities:
 *  - Load JSON configuration file.
 *  - Parse JSON document once.
 *  - Convert JSON data into ServerConfig.
 *  - Validate configuration values.
 *
 * This class is stateless.
 */
class ConfigLoader
{
public:
    /**
     * @brief Load configuration from JSON file.
     *
     * @param filePath Path to configuration file.
     * @return Loaded and validated ServerConfig.
     * @throws std::runtime_error
     */
    [[nodiscard]]
    static ServerConfig load(const std::filesystem::path& filePath);

private:
   
    ConfigLoader() = delete;

    [[nodiscard]] static ServerSettings parseServer(const Json& document);
    [[nodiscard]] static TimingSettings parseTiming(const Json& document);
    [[nodiscard]] static DatabaseSettings parseDatabase(const Json& document);
    [[nodiscard]] static LoggingSettings parseLogging(const Json& document);

   
    static void validate(const ServerConfig& config);
    static void validateServer(const ServerSettings& settings);
    static void validateTiming(const TimingSettings& settings);
    static void validateDatabase(const DatabaseSettings& settings);
    static void validateLogging(const LoggingSettings& settings);
};