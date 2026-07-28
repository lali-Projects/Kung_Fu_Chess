#pragma once

#include <mutex>
#include <string>
#include <vector>
#include <optional>

#include "IDatabase.hpp"

struct sqlite3;
struct sqlite3_stmt;

class SQLiteDatabase : public IDatabase
{
public:
    explicit SQLiteDatabase(const std::string& filename);
    ~SQLiteDatabase() override;

    SQLiteDatabase(const SQLiteDatabase&) = delete;
    SQLiteDatabase& operator=(const SQLiteDatabase&) = delete;

public:
    //=================================
    // Lifecycle
    //=================================
    bool open() override;
    void close() override;
    bool isOpen() const override;

    //=================================
    // Execute
    //=================================
    bool execute(const std::string& sql) override;
    bool execute(const std::string& sql, const std::vector<std::string>& params) override;

    //=================================
    // Query
    //=================================
    std::optional<DatabaseRow> queryOne(
        const std::string& sql,
        const std::vector<std::string>& params
    ) override;

    //=================================
    // Error
    //=================================
    std::string getLastError() const override;

private:
    bool bindParameters(sqlite3_stmt* statement, const std::vector<std::string>& params);

private:
    mutable std::mutex m_mutex;
    sqlite3* m_database{nullptr};
    std::string m_filename;
    std::string m_lastError;
};