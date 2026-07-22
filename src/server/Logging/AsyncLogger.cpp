#include "AsyncLogger.hpp"

#include <stdexcept>

#include "LogFormatter.hpp"

//================================================
// Constructor
//================================================
AsyncLogger::AsyncLogger(
    const std::filesystem::path& logFilePath)
    : m_output(logFilePath, std::ios::out | std::ios::app)
{
    if (!m_output.is_open())
    {
        throw std::runtime_error(
            "Failed to open log file: " +
            logFilePath.string());
    }

    m_worker = std::thread(
        &AsyncLogger::workerLoop,
        this);
}

//================================================
// Destructor
//================================================
AsyncLogger::~AsyncLogger()
{
    stop();
}

//================================================
// Public Log
//================================================
void AsyncLogger::log(
    LogLevel level,
    std::string message)
{
    {
        std::lock_guard<std::mutex> lock(m_mutex);

        if (!m_running)
        {
            return;
        }

        m_queue.emplace(
            level,
            std::move(message));
    }

    m_condition.notify_one();
}

//================================================
// Stop
//================================================
void AsyncLogger::stop()
{
    {
        std::lock_guard<std::mutex> lock(m_mutex);

        if (!m_running)
        {
            return;
        }

        m_running = false;
    }

    m_condition.notify_one();

    if (m_worker.joinable())
    {
        m_worker.join();
    }

    if (m_output.is_open())
    {
        m_output.flush();
        m_output.close();
    }
}

//================================================
// Worker Thread
//================================================
void AsyncLogger::workerLoop()
{
    while (true)
    {
        LogMessage message(
            LogLevel::Trace,
            "");

        {
            std::unique_lock<std::mutex> lock(m_mutex);

            m_condition.wait(
                lock,
                [this]
                {
                    return
                        !m_queue.empty() ||
                        !m_running;
                });

            if (!m_running &&
                m_queue.empty())
            {
                break;
            }

            message =
                std::move(m_queue.front());

            m_queue.pop();
        }

        m_output
            << LogFormatter::format(message)
            << '\n';

        m_output.flush();
    }
}