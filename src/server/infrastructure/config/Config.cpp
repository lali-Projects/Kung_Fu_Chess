#include "Config.hpp"
#include "ConfigLoader.hpp"


Config::Config(const std::filesystem::path& configFile)
    : m_config(ConfigLoader::load(configFile))
{
}

const ServerConfig& Config::get() const noexcept
{
    return m_config;
}


const ServerSettings& Config::server() const noexcept
{
    return m_config.server;
}

const TimingSettings& Config::timing() const noexcept
{
    return m_config.timing;
}

const DatabaseSettings& Config::database() const noexcept
{
    return m_config.database;
}

const LoggingSettings& Config::logging() const noexcept
{
    return m_config.logging;
}