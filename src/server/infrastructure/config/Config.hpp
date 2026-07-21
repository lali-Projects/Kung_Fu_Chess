#pragma once

#include <filesystem>

#include "ServerConfig.hpp"


/**
 * @brief Central access point for server configuration.
 *
 * Config owns the loaded ServerConfig instance
 * and exposes read-only access to configuration data.
 *
 * Responsibilities:
 *  - Load configuration on construction.
 *  - Store configuration lifetime.
 *  - Provide accessors.
 *
 * Does not:
 *  - Parse JSON.
 *  - Validate configuration.
 *  - Access external systems.
 */
class Config
{
public:

    /**
     * @brief Creates configuration manager.
     *
     * @param configFile Path to JSON configuration file.
     *
     * @throws std::runtime_error
     * If configuration loading fails.
     */
    explicit Config(
        const std::filesystem::path& configFile
    );


    ~Config() = default;


    Config(const Config&) = delete;
    Config& operator=(const Config&) = delete;


    Config(Config&&) noexcept = default;
    Config& operator=(Config&&) noexcept = default;

    [[nodiscard]]
    const ServerConfig& get() const noexcept;


    [[nodiscard]]
    const ServerSettings& server() const noexcept;


    [[nodiscard]]
    const TimingSettings& timing() const noexcept;


    [[nodiscard]]
    const DatabaseSettings& database() const noexcept;


    [[nodiscard]]
    const LoggingSettings& logging() const noexcept;


private:

    ServerConfig m_config;
};