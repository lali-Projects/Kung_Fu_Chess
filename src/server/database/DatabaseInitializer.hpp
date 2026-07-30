#pragma once

class IDatabase;

enum class DatabaseDialect
{
    SQLITE,
    POSTGRESQL
};

/**
 * @brief Initializes database schema.
 *
 * Responsible only for creating
 * required database structure.
 */
class DatabaseInitializer
{
public:
    explicit DatabaseInitializer(
        IDatabase& database,
        DatabaseDialect dialect = DatabaseDialect::SQLITE);

    bool initialize();

private:
    bool createUsersTable();
    bool initializePostgreSQL();
    bool createPostgreSQLMigrationTable();
    bool applyPostgreSQLMigration1();

    IDatabase& m_database;
    DatabaseDialect m_dialect;
};
