#pragma once

#include <chrono>
#include <string>
#include <thread>

#include "LogLevel.hpp"

/**
 * @brief Represents a single log entry.
 *
 * This structure is immutable after construction
 * and contains all information required to
 * format and write a log message.
 */
struct LogMessage
{
    /// Time the log message was created.
    std::chrono::system_clock::time_point timestamp;

    /// Severity level.
    LogLevel level;

    /// Thread that created the message.
    std::thread::id threadId;

    /// Log text.
    std::string text;

    /**
     * @brief Constructs a log message.
     *
     * @param logLevel Severity level.
     * @param message Log text.
     */
    LogMessage(
        LogLevel logLevel,
        std::string message)
        :
        timestamp(std::chrono::system_clock::now()),
        level(logLevel),
        threadId(std::this_thread::get_id()),
        text(std::move(message))
    {
    }
};