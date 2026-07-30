#include <chrono>
#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <string>
#include <utility>

#include "DatabaseInitializer.hpp"
#include "PostgreSQLDatabase.hpp"
#include "User.hpp"
#include "UserRepository.hpp"

namespace
{
    std::string requiredEnvironment(const char* name)
    {
        const char* value = std::getenv(name);
        if (!value || std::string(value).empty())
        {
            throw std::runtime_error(
                std::string("Missing required test environment variable: ") +
                name);
        }

        return value;
    }

    void require(bool condition, const std::string& message)
    {
        if (!condition)
        {
            throw std::runtime_error(message);
        }
    }
}

int main()
{
    try
    {
        PostgreSQLConfiguration configuration;
        configuration.host = requiredEnvironment("KFC_POSTGRES_HOST");
        configuration.port = static_cast<std::uint16_t>(
            std::stoul(requiredEnvironment("KFC_POSTGRES_PORT")));
        configuration.database =
            requiredEnvironment("KFC_POSTGRES_DATABASE");
        configuration.user = requiredEnvironment("KFC_POSTGRES_USER");
        configuration.password =
            requiredEnvironment("KFC_POSTGRES_PASSWORD");

        PostgreSQLDatabase database(std::move(configuration));
        require(
            database.open(),
            "PostgreSQL open failed: " + database.getLastError());

        DatabaseInitializer initializer(
            database,
            DatabaseDialect::POSTGRESQL);
        require(
            initializer.initialize(),
            "PostgreSQL migration failed: " + database.getLastError());
        require(
            initializer.initialize(),
            "Repeated PostgreSQL migration failed: " +
                database.getLastError());

        const auto migration = database.queryOne(
            "SELECT version, name FROM schema_migrations WHERE version=?;",
            {"1"});
        require(migration.has_value(), "Migration 1 was not recorded");
        require(
            migration->at("name") == "create_users",
            "Migration 1 has an unexpected name");

        const auto now = std::chrono::steady_clock::now()
            .time_since_epoch().count();
        std::string suffix = std::to_string(now);
        if (suffix.size() > 10)
        {
            suffix = suffix.substr(suffix.size() - 10);
        }

        const std::string username = "pg_'" + suffix;
        const std::string password = "stage5_test_password";
        UserRepository repository(database);
        const User user("", username, password);

        require(
            repository.create(user),
            "Quoted username insertion failed: " +
                database.getLastError());
        require(
            !repository.create(user),
            "Duplicate username insertion unexpectedly succeeded");

        const auto loaded = repository.findByUsername(username);
        require(loaded.has_value(), "Inserted user was not found");
        require(!loaded->getId().empty(), "Inserted user ID is empty");
        require(
            loaded->getUsername() == username,
            "Loaded username differs from inserted value");
        require(
            loaded->getPassword() == password,
            "Loaded password differs from inserted value");

        database.close();
        require(!database.isOpen(), "PostgreSQL connection did not close");

        std::cout << "PostgreSQL database integration checks passed\n";
        return 0;
    }
    catch (const std::exception& exception)
    {
        std::cerr
            << "PostgreSQL database integration checks failed: "
            << exception.what()
            << '\n';
        return 1;
    }
}
