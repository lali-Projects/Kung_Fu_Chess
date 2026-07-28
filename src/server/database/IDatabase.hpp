#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <optional>

using DatabaseRow = std::unordered_map<std::string, std::string>;

class IDatabase
{
public:
    virtual ~IDatabase() = default;

public:
    //=================================
    // Lifecycle
    //=================================
    virtual bool open() = 0;
    virtual void close() = 0;
    virtual bool isOpen() const = 0;

    //=================================
    // Commands
    //=================================
    virtual bool execute(const std::string& sql) = 0;
    virtual bool execute(const std::string& sql, const std::vector<std::string>& params) = 0;

    //=================================
    // Query
    //=================================
    virtual std::optional<DatabaseRow> queryOne(const std::string& sql, const std::vector<std::string>& params) = 0;

    //=================================
    // Error
    //=================================
    virtual std::string getLastError() const = 0;
};