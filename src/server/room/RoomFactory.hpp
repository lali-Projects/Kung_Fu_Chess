#pragma once

#include <memory>
#include <string>

class Room;
class EventBus;
/**
 * @brief Creates fully initialized game rooms and their required dependencies.
 *
 * Builds the GameContext and GameSession for each room and transfers their
 * ownership to the created Room.
 */
class RoomFactory
{
public:
    explicit RoomFactory(EventBus& eventBus);

    std::unique_ptr<Room> createRoom(const std::string& roomId);

private:
    EventBus& m_eventBus;
};