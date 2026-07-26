#include "RoomManager.hpp"


#include <stdexcept>


#include "Room.hpp"
#include "RoomFactory.hpp"




//================================================
// Constructor
//================================================

RoomManager::RoomManager(
    RoomFactory& factory)
:
m_factory(factory)
{
}





RoomManager::~RoomManager() = default;






//================================================
// Create Room
//================================================

bool RoomManager::createRoom(
    const std::string& id)
{

    if(id.empty())
    {
        return false;
    }



    {
        std::lock_guard<std::mutex> lock(
            m_mutex);


        if(existsUnsafe(id))
        {
            return false;
        }
    }



    auto room =
        m_factory.createRoom(id);



    if(!room)
    {
        return false;
    }



    {
        std::lock_guard<std::mutex> lock(
            m_mutex);



        if(existsUnsafe(id))
        {
            return false;
        }



        m_rooms.emplace(
            id,
            std::move(room));
    }



    return true;
}







//================================================
// Remove Room
//================================================

bool RoomManager::removeRoom(
    const std::string& id)
{

    std::lock_guard<std::mutex> lock(
        m_mutex);



    return
        m_rooms.erase(id)
        >
        0;
}







//================================================
// Get Room
//================================================

Room*
RoomManager::getRoom(
    const std::string& id)
{

    std::lock_guard<std::mutex> lock(
        m_mutex);



    auto iterator =
        m_rooms.find(id);



    if(iterator == m_rooms.end())
    {
        return nullptr;
    }



    return iterator->second.get();
}







const Room*
RoomManager::getRoom(
    const std::string& id) const
{

    std::lock_guard<std::mutex> lock(
        m_mutex);



    auto iterator =
        m_rooms.find(id);



    if(iterator == m_rooms.end())
    {
        return nullptr;
    }



    return iterator->second.get();
}







//================================================
// Get Or Create
//================================================

Room&
RoomManager::getOrCreateRoom(
    const std::string& id)
{

    {
        std::lock_guard<std::mutex> lock(
            m_mutex);



        auto iterator =
            m_rooms.find(id);



        if(iterator != m_rooms.end())
        {
            return *iterator->second;
        }
    }



    if(!createRoom(id))
    {
        throw std::runtime_error(
            "Cannot create room: " + id);
    }



    Room* room =
        getRoom(id);



    if(!room)
    {
        throw std::runtime_error(
            "Room creation failed");
    }



    return *room;
}







//================================================
// Exists
//================================================

bool RoomManager::exists(
    const std::string& id) const
{

    std::lock_guard<std::mutex> lock(
        m_mutex);



    return existsUnsafe(id);
}







bool RoomManager::existsUnsafe(const std::string& id) const
{
    return m_rooms.find(id) != m_rooms.end();
}







//================================================
// Count
//================================================

size_t RoomManager::roomCount() const
{

    std::lock_guard<std::mutex> lock(
        m_mutex);



    return m_rooms.size();
}