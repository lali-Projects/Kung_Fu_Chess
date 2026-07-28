#pragma once

#include <cstddef>
#include <memory>
#include <mutex>
#include <string>
#include <unordered_map>
#include <vector>

class Room;
class RoomFactory;

class RoomManager
{
public:
    explicit RoomManager(RoomFactory& factory);

    ~RoomManager();

    RoomManager(const RoomManager&) = delete;
    RoomManager& operator=(const RoomManager&) = delete;

public:
    bool createRoom(const std::string& id);

    bool removeRoom(const std::string& id);

    void clearRooms();

public:
    std::shared_ptr<Room> getRoom(const std::string& id);

    std::shared_ptr<const Room> getRoom(const std::string& id) const;

    std::shared_ptr<Room> getOrCreateRoom(const std::string& id);

public:
    bool exists(const std::string& id) const;

    size_t roomCount() const;

    std::vector<std::string> getRoomIds() const;

    std::vector<std::shared_ptr<Room>> getRoomsSnapshot() const;

private:
    bool validateRoomId(const std::string& id) const;

    bool existsUnsafe(const std::string& id) const;

private:
    RoomFactory& m_factory;

    mutable std::mutex m_mutex;

    std::unordered_map<std::string, std::shared_ptr<Room>> m_rooms;
};