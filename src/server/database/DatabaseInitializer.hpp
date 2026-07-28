#pragma once

class IDatabase;

/**
 * @brief Initializes database schema.
 *
 * Responsible only for creating
 * required database structure.
 */
class DatabaseInitializer
{
public:
    explicit DatabaseInitializer(IDatabase& database);

    bool initialize();

private:
    bool createUsersTable();

    IDatabase& m_database;
};