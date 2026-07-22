#pragma once

#include <string_view>

/**
 * @brief Severity level of a log message.
 *
 * The values are ordered from the lowest severity
 * to the highest severity.
 */
enum class LogLevel
{
    Trace,
    Debug,
    Info,
    Warning,
    Error,
    Critical
};

/**
 * @brief Converts a LogLevel to its textual representation.
 *
 * @param level Log level.
 *
 * @return Constant string representation.
 */
[[nodiscard]]
constexpr std::string_view toString(LogLevel level) noexcept
{
    switch (level)
    {
    case LogLevel::Trace:
        return "TRACE";

    case LogLevel::Debug:
        return "DEBUG";

    case LogLevel::Info:
        return "INFO";

    case LogLevel::Warning:
        return "WARNING";

    case LogLevel::Error:
        return "ERROR";

    case LogLevel::Critical:
        return "CRITICAL";
    }

    return "UNKNOWN";
}