#include "DatabaseInitializer.hpp"

#include "IDatabase.hpp"

//================================================
// Constructor
//================================================

DatabaseInitializer::DatabaseInitializer(
    IDatabase& database,
    DatabaseDialect dialect)
    : m_database(database),
      m_dialect(dialect)
{
}

//================================================
// Initialize
//================================================

bool DatabaseInitializer::initialize()
{
    if (m_dialect == DatabaseDialect::POSTGRESQL)
    {
        return initializePostgreSQL();
    }

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

//================================================
// PostgreSQL Migrations
//================================================

bool DatabaseInitializer::initializePostgreSQL()
{
    if (!createPostgreSQLMigrationTable())
    {
        return false;
    }

    const auto migration = m_database.queryOne(
        "SELECT version FROM schema_migrations WHERE version=?;",
        {"1"});

    if (migration)
    {
        return true;
    }

    if (!m_database.getLastError().empty())
    {
        return false;
    }

    return applyPostgreSQLMigration1();
}

bool DatabaseInitializer::createPostgreSQLMigrationTable()
{
    const std::string sql = R"(
        CREATE TABLE IF NOT EXISTS schema_migrations (
            version INTEGER PRIMARY KEY,
            name TEXT NOT NULL,
            applied_at TIMESTAMPTZ NOT NULL DEFAULT CURRENT_TIMESTAMP
        );
    )";

    return m_database.execute(sql);
}

bool DatabaseInitializer::applyPostgreSQLMigration1()
{
    const std::string sql = R"(
        DO $migration$
        BEGIN
            CREATE TABLE IF NOT EXISTS users (
                id BIGSERIAL PRIMARY KEY,
                username TEXT NOT NULL UNIQUE,
                password TEXT NOT NULL
            );

            INSERT INTO schema_migrations (version, name)
            VALUES (1, 'create_users');
        END
        $migration$;
    )";

    return m_database.execute(sql);
}
