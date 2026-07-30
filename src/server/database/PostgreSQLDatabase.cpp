#include "PostgreSQLDatabase.hpp"

#include <libpq-fe.h>

#include <utility>

struct PostgreSQLDatabase::Impl
{
    PGconn* connection{nullptr};
};

PostgreSQLDatabase::PostgreSQLDatabase(
    PostgreSQLConfiguration configuration)
    : m_configuration(std::move(configuration)),
      m_impl(std::make_unique<Impl>())
{
}

PostgreSQLDatabase::~PostgreSQLDatabase()
{
    close();
}

bool PostgreSQLDatabase::open()
{
    std::lock_guard<std::mutex> lock(m_mutex);
    m_lastError.clear();

    return ensureConnectedUnlocked();
}

bool PostgreSQLDatabase::connectUnlocked()
{
    m_lastError.clear();

    if (m_impl->connection)
    {
        PQfinish(m_impl->connection);
        m_impl->connection = nullptr;
    }

    const std::string port = std::to_string(m_configuration.port);
    const char* keywords[] =
    {
        "host",
        "port",
        "dbname",
        "user",
        "password",
        "connect_timeout",
        nullptr
    };
    const char* values[] =
    {
        m_configuration.host.c_str(),
        port.c_str(),
        m_configuration.database.c_str(),
        m_configuration.user.c_str(),
        m_configuration.password.c_str(),
        "5",
        nullptr
    };

    m_impl->connection = PQconnectdbParams(keywords, values, 0);

    if (!m_impl->connection ||
        PQstatus(m_impl->connection) != CONNECTION_OK)
    {
        m_lastError = m_impl->connection
            ? sanitizeError(PQerrorMessage(m_impl->connection))
            : "PostgreSQL connection allocation failed";

        if (m_impl->connection)
        {
            PQfinish(m_impl->connection);
            m_impl->connection = nullptr;
        }

        return false;
    }

    return true;
}

bool PostgreSQLDatabase::ensureConnectedUnlocked()
{
    if (m_impl->connection &&
        PQstatus(m_impl->connection) == CONNECTION_OK)
    {
        return true;
    }

    return connectUnlocked();
}

void PostgreSQLDatabase::close()
{
    std::lock_guard<std::mutex> lock(m_mutex);

    if (!m_impl->connection)
    {
        return;
    }

    PQfinish(m_impl->connection);
    m_impl->connection = nullptr;
}

bool PostgreSQLDatabase::isOpen() const
{
    std::lock_guard<std::mutex> lock(m_mutex);
    return m_impl->connection &&
        PQstatus(m_impl->connection) == CONNECTION_OK;
}

bool PostgreSQLDatabase::execute(const std::string& sql)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    m_lastError.clear();

    if (!ensureConnectedUnlocked())
    {
        return false;
    }

    PGresult* result = PQexec(m_impl->connection, sql.c_str());
    const bool succeeded = result &&
        PQresultStatus(result) == PGRES_COMMAND_OK;

    if (!succeeded)
    {
        m_lastError = result
            ? sanitizeError(PQresultErrorMessage(result))
            : sanitizeError(PQerrorMessage(m_impl->connection));
    }

    if (result)
    {
        PQclear(result);
    }

    return succeeded;
}

bool PostgreSQLDatabase::execute(
    const std::string& sql,
    const std::vector<std::string>& params)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    m_lastError.clear();

    if (!ensureConnectedUnlocked())
    {
        return false;
    }

    const auto translatedSql =
        translatePlaceholders(sql, params.size());
    if (!translatedSql)
    {
        return false;
    }

    std::vector<const char*> parameterValues;
    parameterValues.reserve(params.size());
    for (const auto& parameter : params)
    {
        parameterValues.push_back(parameter.c_str());
    }

    PGresult* result = PQexecParams(
        m_impl->connection,
        translatedSql->c_str(),
        static_cast<int>(parameterValues.size()),
        nullptr,
        parameterValues.empty() ? nullptr : parameterValues.data(),
        nullptr,
        nullptr,
        0);

    const bool succeeded = result &&
        PQresultStatus(result) == PGRES_COMMAND_OK;

    if (!succeeded)
    {
        m_lastError = result
            ? sanitizeError(PQresultErrorMessage(result))
            : sanitizeError(PQerrorMessage(m_impl->connection));
    }

    if (result)
    {
        PQclear(result);
    }

    return succeeded;
}

std::optional<DatabaseRow> PostgreSQLDatabase::queryOne(
    const std::string& sql,
    const std::vector<std::string>& params)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    m_lastError.clear();

    if (!ensureConnectedUnlocked())
    {
        return std::nullopt;
    }

    const auto translatedSql =
        translatePlaceholders(sql, params.size());
    if (!translatedSql)
    {
        return std::nullopt;
    }

    std::vector<const char*> parameterValues;
    parameterValues.reserve(params.size());
    for (const auto& parameter : params)
    {
        parameterValues.push_back(parameter.c_str());
    }

    const auto executeQuery = [&]()
    {
        return PQexecParams(
            m_impl->connection,
            translatedSql->c_str(),
            static_cast<int>(parameterValues.size()),
            nullptr,
            parameterValues.empty() ? nullptr : parameterValues.data(),
            nullptr,
            nullptr,
            0);
    };

    PGresult* result = executeQuery();

    if ((!result || PQresultStatus(result) != PGRES_TUPLES_OK) &&
        (!m_impl->connection ||
         PQstatus(m_impl->connection) != CONNECTION_OK))
    {
        if (result)
        {
            PQclear(result);
            result = nullptr;
        }

        if (connectUnlocked())
        {
            result = executeQuery();
        }
    }

    if (!result || PQresultStatus(result) != PGRES_TUPLES_OK)
    {
        if (result)
        {
            m_lastError = sanitizeError(PQresultErrorMessage(result));
        }
        else if (m_impl->connection)
        {
            m_lastError = sanitizeError(PQerrorMessage(m_impl->connection));
        }
        else if (m_lastError.empty())
        {
            m_lastError = "PostgreSQL query failed";
        }

        if (result)
        {
            PQclear(result);
        }

        return std::nullopt;
    }

    if (PQntuples(result) == 0)
    {
        PQclear(result);
        return std::nullopt;
    }

    DatabaseRow row;
    const int columns = PQnfields(result);

    for (int column = 0; column < columns; ++column)
    {
        const char* name = PQfname(result, column);
        const char* value = PQgetisnull(result, 0, column)
            ? ""
            : PQgetvalue(result, 0, column);
        row[name ? name : ""] = value ? value : "";
    }

    PQclear(result);
    return row;
}

std::string PostgreSQLDatabase::getLastError() const
{
    std::lock_guard<std::mutex> lock(m_mutex);
    return m_lastError;
}

std::optional<std::string> PostgreSQLDatabase::translatePlaceholders(
    const std::string& sql,
    std::size_t parameterCount)
{
    std::string translated;
    translated.reserve(sql.size() + parameterCount * 2);

    bool insideSingleQuote = false;
    bool insideDoubleQuote = false;
    std::size_t placeholderCount = 0;

    for (std::size_t index = 0; index < sql.size(); ++index)
    {
        const char character = sql[index];

        if (character == '\'' && !insideDoubleQuote)
        {
            translated.push_back(character);

            if (insideSingleQuote &&
                index + 1 < sql.size() &&
                sql[index + 1] == '\'')
            {
                translated.push_back(sql[++index]);
                continue;
            }

            insideSingleQuote = !insideSingleQuote;
            continue;
        }

        if (character == '"' && !insideSingleQuote)
        {
            translated.push_back(character);

            if (insideDoubleQuote &&
                index + 1 < sql.size() &&
                sql[index + 1] == '"')
            {
                translated.push_back(sql[++index]);
                continue;
            }

            insideDoubleQuote = !insideDoubleQuote;
            continue;
        }

        if (character == '?' &&
            !insideSingleQuote &&
            !insideDoubleQuote)
        {
            translated += "$" + std::to_string(++placeholderCount);
            continue;
        }

        translated.push_back(character);
    }

    if (insideSingleQuote || insideDoubleQuote)
    {
        m_lastError = "Unterminated SQL quoted value";
        return std::nullopt;
    }

    if (placeholderCount != parameterCount)
    {
        m_lastError = "SQL parameter count does not match placeholders";
        return std::nullopt;
    }

    return translated;
}

std::string PostgreSQLDatabase::sanitizeError(const char* error) const
{
    std::string sanitized = error ? error : "PostgreSQL operation failed";

    if (!m_configuration.password.empty())
    {
        std::size_t position = 0;
        while ((position = sanitized.find(
                    m_configuration.password,
                    position)) != std::string::npos)
        {
            sanitized.replace(
                position,
                m_configuration.password.size(),
                "[REDACTED]");
            position += 10;
        }
    }

    return sanitized;
}
