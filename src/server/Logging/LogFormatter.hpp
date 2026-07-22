#pragma once

#include <string>

#include "LogMessage.hpp"

/**
 * @brief Converts LogMessage objects into formatted strings.
 *
 * This class is stateless and contains only formatting logic.
 */
class LogFormatter
{
public:

    /**
     * @brief Formats a log message.
     *
     * Example:
     *
     * 2026-07-21 18:32:15.245
     * [INFO]
     * [Thread 12]
     * Server started
     */
    [[nodiscard]]
    static std::string format(
        const LogMessage& message
    );

private:

    LogFormatter() = delete;

    [[nodiscard]]
    static std::string formatTimestamp(
        std::chrono::system_clock::time_point timePoint
    );

    [[nodiscard]]
    static std::string formatThreadId(
        std::thread::id id
    );
};