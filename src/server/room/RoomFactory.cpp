#include "RoomFactory.hpp"

#include "Room.hpp"
#include "GameContext.hpp"
#include "GameSession.hpp"
#include "EventBus.hpp"

#include <exception>

RoomFactory::RoomFactory(EventBus& eventBus)
    : m_eventBus(eventBus)
{
}

std::unique_ptr<Room> RoomFactory::createRoom(const std::string& roomId)
{
    if (roomId.empty())
        return nullptr;

    try
    {
        auto context = std::make_unique<GameContext>();

        auto session = std::make_unique<GameSession>(
            roomId,
            *context,
            m_eventBus
        );

        return std::make_unique<Room>(
            roomId,
            std::move(context),
            std::move(session)
        );
    }
    catch (...)
    {
        return nullptr;
    }
}