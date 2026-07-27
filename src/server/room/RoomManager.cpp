#include "RoomManager.hpp"


#include "Room.hpp"
#include "RoomFactory.hpp"


#include <cctype>
#include <stdexcept>





//================================================
// Constructor
//================================================

RoomManager::RoomManager(
    RoomFactory& factory)
:
m_factory(factory)
{
}





//================================================
// Destructor
//================================================

RoomManager::~RoomManager()
{
    clearRooms();
}








//================================================
// Create Room
//================================================

bool RoomManager::createRoom(
    const std::string& id)
{

    if(!validateRoomId(id))
        return false;



    {
        std::lock_guard<std::mutex> lock(
            m_mutex);


        if(existsUnsafe(id))
            return false;
    }




    auto room =
        m_factory.createRoom(id);



    if(!room)
        return false;




    std::lock_guard<std::mutex> lock(
        m_mutex);



    if(existsUnsafe(id))
        return false;



    m_rooms.emplace(
        id,
        std::move(room));



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
// Clear
//================================================

void RoomManager::clearRooms()
{

    std::lock_guard<std::mutex> lock(
        m_mutex);



    m_rooms.clear();

}








//================================================
// Get Room
//================================================

std::shared_ptr<Room>
RoomManager::getRoom(
    const std::string& id)
{

    std::lock_guard<std::mutex> lock(
        m_mutex);



    auto iterator =
        m_rooms.find(id);



    if(iterator == m_rooms.end())
        return nullptr;



    return iterator->second;

}








std::shared_ptr<const Room>
RoomManager::getRoom(
    const std::string& id) const
{

    std::lock_guard<std::mutex> lock(
        m_mutex);



    auto iterator =
        m_rooms.find(id);



    if(iterator == m_rooms.end())
        return nullptr;



    return iterator->second;

}









//================================================
// Get Or Create
//================================================
std::shared_ptr<Room>
RoomManager::getOrCreateRoom(
    const std::string& id)
{

    if(!validateRoomId(id))
        return nullptr;



    std::lock_guard<std::mutex> lock(
        m_mutex);



    auto iterator =
        m_rooms.find(id);



    if(iterator != m_rooms.end())
    {
        return iterator->second;
    }





    auto room =
        m_factory.createRoom(id);



    if(!room)
        return nullptr;



    std::shared_ptr<Room> sharedRoom(
        std::move(room));


    m_rooms.emplace(
        id,
        sharedRoom);



    return sharedRoom;

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








bool RoomManager::existsUnsafe(
    const std::string& id) const
{

    return
        m_rooms.find(id)
        !=
        m_rooms.end();

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








//================================================
// IDs
//================================================

std::vector<std::string>
RoomManager::getRoomIds() const
{

    std::vector<std::string> result;



    std::lock_guard<std::mutex> lock(
        m_mutex);



    for(const auto& [id, room] : m_rooms)
    {
        result.push_back(id);
    }



    return result;

}


std::vector<std::shared_ptr<Room>>
RoomManager::getRoomsSnapshot() const
{
    std::vector<std::shared_ptr<Room>> result;


    std::lock_guard<std::mutex> lock(m_mutex);


    result.reserve(m_rooms.size());


    for(const auto& [id, room] : m_rooms)
    {
        if(room)
        {
            result.push_back(room);
        }
    }


    return result;
}

bool RoomManager::validateRoomId(
    const std::string& id) const
{

    if(id.empty())
        return false;


    for(char c : id)
    {
        if(!std::isspace(
            static_cast<unsigned char>(c)))
        {
            return true;
        }
    }


    return false;
}
