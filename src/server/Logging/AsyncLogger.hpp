#pragma once

#include <condition_variable>
#include <filesystem>
#include <fstream>
#include <mutex>
#include <queue>
#include <string>
#include <thread>

#include "LogLevel.hpp"
#include "LogMessage.hpp"

/**
 * @brief Asynchronous file logger.
 *
 * Thread-safe asynchronous logger that receives log messages
 * from multiple producer threads and writes them to a log file
 * using a dedicated background thread.
 *
 * Responsibilities:
 *  - Open and own the log file.
 *  - Accept log messages from multiple threads.
 *  - Store messages in a thread-safe queue.
 *  - Write messages asynchronously.
 *  - Flush and stop gracefully during destruction.
 *
 * This class does NOT:
 *  - Format log messages (LogFormatter).
 *  - Define log levels (LogLevel).
 *  - Manage configuration.
 */
class AsyncLogger
{
public:

    /**
     * @brief Creates the logger and starts the worker thread.
     *
     * @param logFilePath Path to the log file.
     *
     * @throws std::runtime_error
     * If the file cannot be opened.
     */
    explicit AsyncLogger(
        const std::filesystem::path& logFilePath);

    /**
     * @brief Stops the worker thread and flushes all pending logs.
     */
    ~AsyncLogger();

    AsyncLogger(const AsyncLogger&) = delete;
    AsyncLogger& operator=(const AsyncLogger&) = delete;

    AsyncLogger(AsyncLogger&&) = delete;
    AsyncLogger& operator=(AsyncLogger&&) = delete;

    /**
     * @brief Adds a log message to the queue.
     *
     * This function is safe to call concurrently
     * from multiple threads.
     *
     * @param level Severity level.
     * @param message Log message.
     */
    void log(
        LogLevel level,
        std::string message);

private:

    /**
     * @brief Background thread entry point.
     */
    void workerLoop();

    /**
     * @brief Stops the logger.
     *
     * Safe to call multiple times.
     */
    void stop();

private:

    /// Output file stream.
    std::ofstream m_output;

    /// Queue of pending log messages.
    std::queue<LogMessage> m_queue;

    /// Protects both the queue and the running flag.
    mutable std::mutex m_mutex;

    /// Signals the worker thread that new work is available.
    std::condition_variable m_condition;

    /// Dedicated logging thread.
    std::thread m_worker;

    /// Protected by m_mutex.
    bool m_running{true};
};