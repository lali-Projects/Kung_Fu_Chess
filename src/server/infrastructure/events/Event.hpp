#pragma once

/**
 * @brief Base class for all system events.
 *
 * Events are data objects only.
 * They do not contain business logic.
 */
class Event
{
public:

    virtual ~Event() = default;
};