#pragma once


#include <cstddef>
#include <memory>
#include <mutex>
#include <string>
#include <unordered_map>



class Room;
class RoomFactory;



/**
 * @brief Manages active game rooms.
 */
class RoomManager
{

public:


    explicit RoomManager(
        RoomFactory& factory);



    ~RoomManager();



    RoomManager(
        const RoomManager&) = delete;



    RoomManager& operator=(
        const RoomManager&) = delete;



public:


    bool createRoom(
        const std::string& id);



    bool removeRoom(
        const std::string& id);



    Room* getRoom(
        const std::string& id);



    const Room* getRoom(
        const std::string& id) const;



    Room& getOrCreateRoom(
        const std::string& id);



    bool exists(
        const std::string& id) const;



    size_t roomCount() const;



private:


    bool existsUnsafe(
        const std::string& id) const;



private:


    RoomFactory& m_factory;



    mutable std::mutex m_mutex;



    std::unordered_map<
        std::string,
        std::unique_ptr<Room>>
        m_rooms;

};