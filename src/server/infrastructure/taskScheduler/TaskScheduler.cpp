#include "TaskScheduler.hpp"
//================================================
// Constructor
//================================================

TaskScheduler::TaskScheduler()
{
    try
    {
        // Start the dedicated worker thread.
        m_worker = std::thread(
            &TaskScheduler::workerLoop,
            this);
    }
    catch (...)
    {
        // Leave the object in a valid state and
        // propagate the exception to the caller.
        m_running = false;
        throw;
    }
}

//================================================
// Destructor
//================================================

TaskScheduler::~TaskScheduler() noexcept
{
    try
    {
        stop();
    }
    catch (...)
    {
        // A destructor must never allow exceptions
        // to escape.
    }
}

//================================================
// Schedule Task
//================================================

void TaskScheduler::schedule(Task task)
{
    // Reject empty tasks.
    if (!task)
    {
        throw std::invalid_argument(
            "TaskScheduler::schedule received an empty task.");
    }


    {
        // Protect queue and running state.
        std::lock_guard<std::mutex> lock(m_mutex);


        // Do not accept new tasks after shutdown.
        if (!m_running)
        {
            throw std::runtime_error(
                "Cannot schedule task after TaskScheduler has stopped.");
        }


        // Add task to FIFO queue.
        // Move avoids unnecessary copy of std::function.
        m_tasks.emplace(std::move(task));
    }


    // Wake the worker thread.
    // Must be called after releasing the mutex.
    m_condition.notify_one();
}

//================================================
// Stop Scheduler
//================================================

void TaskScheduler::stop()
{
    {
        // Protect access to running state.
        std::lock_guard<std::mutex> lock(m_mutex);

        // Prevent multiple shutdown attempts.
        if (!m_running)
        {
            return;
        }

        // Mark scheduler as stopping.
        m_running = false;
    }


    // Wake the worker thread.
    //
    // It may currently be waiting on
    // condition_variable because the queue is empty.
    m_condition.notify_one();


    // Wait until workerLoop() finishes.
    //
    // join() guarantees that the thread resources
    // are released before the object is destroyed.
    if (m_worker.joinable())
    {
        m_worker.join();
    }
}

//================================================
// Worker Thread Loop
//================================================

void TaskScheduler::workerLoop()
{
    while (true)
    {
        Task currentTask;


        {
            std::unique_lock<std::mutex> lock(m_mutex);


            // Wait until:
            // 1. A new task arrives
            // 2. Shutdown is requested
            m_condition.wait(
                lock,
                [this]()
                {
                    return
                        !m_tasks.empty() ||
                        !m_running;
                });


            // Exit only when:
            // - Scheduler is stopped
            // - No remaining tasks exist
            if (!m_running &&
                m_tasks.empty())
            {
                break;
            }


            // Move the next task from the queue.
            currentTask =
                std::move(m_tasks.front());

            m_tasks.pop();
        }


        // Execute outside the mutex.
        //
        // This is important:
        // A long-running task must not block
        // other threads from scheduling new tasks.
        try
        {
            currentTask();
        }
        catch (const std::exception&)
        {
            // Future:
            // Report exception using AsyncLogger.
        }
        catch (...)
        {
            // Future:
            // Report unknown exception.
        }
    }
}