#include "DatabaseInitializer.hpp"

#include "IDatabase.hpp"

//================================================
// Constructor
//================================================

DatabaseInitializer::DatabaseInitializer(IDatabase& database)
    : m_database(database)
{
}

//================================================
// Initialize
//================================================

bool DatabaseInitializer::initialize()
{
    return createUsersTable();
}

//================================================
// Create Users Table
//================================================

bool DatabaseInitializer::createUsersTable()
{
    const std::string sql = R"(
        CREATE TABLE IF NOT EXISTS users (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            username TEXT NOT NULL UNIQUE,
            password TEXT NOT NULL
        );
    )";

    return m_database.execute(sql);
}