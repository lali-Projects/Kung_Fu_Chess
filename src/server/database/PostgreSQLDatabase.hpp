#pragma once

#include <memory>
#include <mutex>
#include <optional>
#include <string>
#include <vector>

#include "DatabaseConfiguration.hpp"
#include "IDatabase.hpp"

class PostgreSQLDatabase final : public IDatabase
{
public:
    explicit PostgreSQLDatabase(PostgreSQLConfiguration configuration);
    ~PostgreSQLDatabase() override;

    PostgreSQLDatabase(const PostgreSQLDatabase&) = delete;
    PostgreSQLDatabase& operator=(const PostgreSQLDatabase&) = delete;

    bool open() override;
    void close() override;
    bool isOpen() const override;

    bool execute(const std::string& sql) override;
    bool execute(
        const std::string& sql,
        const std::vector<std::string>& params) override;

    std::optional<DatabaseRow> queryOne(
        const std::string& sql,
        const std::vector<std::string>& params) override;

    std::string getLastError() const override;

private:
    struct Impl;

    bool connectUnlocked();
    bool ensureConnectedUnlocked();
    std::optional<std::string> translatePlaceholders(
        const std::string& sql,
        std::size_t parameterCount);
    std::string sanitizeError(const char* error) const;

    mutable std::mutex m_mutex;
    PostgreSQLConfiguration m_configuration;
    std::unique_ptr<Impl> m_impl;
    std::string m_lastError;
};
