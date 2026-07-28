#include "EventBus.hpp"


//================================================
// Publish Event
//================================================

void EventBus::publish(
    std::shared_ptr<Event> event)
{
    if (!event)
    {
        return;
    }


    std::vector<EventCallback> listeners;


    {
        std::lock_guard<std::mutex> lock(m_mutex);


        auto iterator =
            m_subscribers.find(
                std::type_index(typeid(*event)));


        if(iterator == m_subscribers.end())
        {
            return;
        }


        /*
            Copy listeners before execution.

            We do not execute callbacks while holding
            the mutex.

            Reason:
            A callback may:
             - Publish another event.
             - Register another listener.
             - Execute long operations.

            Holding the lock could cause deadlocks
            or block other threads.
        */
        listeners = iterator->second;
    }


    for(auto& callback : listeners)
    {
        callback(event);
    }
}