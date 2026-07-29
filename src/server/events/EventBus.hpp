#pragma once

#include <functional>
#include <memory>
#include <mutex>
#include <typeindex>
#include <unordered_map>
#include <vector>

#include "Event.hpp"

/**
 * @brief Internal event distribution system.
 *
 * Implements Observer Pattern.
 *
 * Responsibilities:
 *  - Register event listeners.
 *  - Publish events.
 *  - Notify subscribers.
 */
class EventBus
{
public:
    using EventCallback = std::function<void(std::shared_ptr<Event>)>;

public:
    EventBus() = default;
    ~EventBus() = default;

    EventBus(const EventBus&) = delete;
    EventBus& operator=(const EventBus&) = delete;

    EventBus(EventBus&&) = delete;
    EventBus& operator=(EventBus&&) = delete;

    template<typename EventType>
    void subscribe(std::function<void(std::shared_ptr<EventType>)> callback);
    void publish(std::shared_ptr<Event> event);

private:
    
    std::unordered_map<std::type_index, std::vector<EventCallback>> m_subscribers;

    std::mutex m_mutex;
};

//================================================
// Subscribe Implementation
//================================================

template<typename EventType>
void EventBus::subscribe(std::function<void(std::shared_ptr<EventType>)> callback)
{
    if (!callback)
    {
        return;
    }

    std::lock_guard<std::mutex> lock(m_mutex);

    auto wrapper = [callback](std::shared_ptr<Event> event)
    {
        callback(std::static_pointer_cast<EventType>(event));
    };

    m_subscribers[std::type_index(typeid(EventType))].push_back(std::move(wrapper));
}