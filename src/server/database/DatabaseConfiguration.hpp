#pragma once

#include <cstdint>
#include <string>

enum class DatabaseType
{
    SQLITE,
    POSTGRESQL
};

struct PostgreSQLConfiguration
{
    std::string host;
    std::uint16_t port{5432};
    std::string database;
    std::string user;
    std::string password;
};

struct DatabaseConfiguration
{
    DatabaseType type{DatabaseType::SQLITE};
    std::string sqlitePath{"kungfu_chess.db"};
    PostgreSQLConfiguration postgresql;
};
