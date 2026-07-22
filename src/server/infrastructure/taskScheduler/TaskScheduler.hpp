#pragma once

#include <condition_variable>
#include <functional>
#include <mutex>
#include <queue>
#include <thread>

/**
 * @brief Executes tasks asynchronously using a dedicated worker thread.
 *
 * Responsibilities:
 *  - Accept tasks from multiple threads.
 *  - Store tasks in a thread-safe queue.
 *  - Execute tasks sequentially.
 *  - Shutdown gracefully.
 *
 * This class does NOT know:
 *  - What the task does.
 *  - Who submitted the task.
 *  - Anything about GameEngine, Logger or Database.
 */
class TaskScheduler
{
public:

    using Task = std::function<void()>;

    /**
     * @brief Creates the scheduler and starts the worker thread.
     */
    TaskScheduler();

    /**
     * @brief Stops the worker thread after executing
     * all queued tasks.
     */
    ~TaskScheduler() noexcept;

    TaskScheduler(const TaskScheduler&) = delete;
    TaskScheduler& operator=(const TaskScheduler&) = delete;

    TaskScheduler(TaskScheduler&&) = delete;
    TaskScheduler& operator=(TaskScheduler&&) = delete;

    /**
     * @brief Adds a new task to the execution queue.
     *
     * Thread-safe.
     *
     * @param task Task to execute asynchronously.
     */
    void schedule(Task task);

private:

    /**
     * @brief Main worker loop.
     *
     * Waits for new tasks and executes them.
     */
    void workerLoop();

    /**
     * @brief Stops the scheduler.
     *
     * Safe to call multiple times.
     */
    void stop();

private:

    /// Queue of pending tasks.
    std::queue<Task> m_tasks;

    /// Protects the queue and running state.
    std::mutex m_mutex;

    /// Wakes the worker thread when new tasks arrive.
    std::condition_variable m_condition;

    /// Dedicated worker thread.
    std::thread m_worker;

    /// Protected by m_mutex.
    bool m_running{true};
};