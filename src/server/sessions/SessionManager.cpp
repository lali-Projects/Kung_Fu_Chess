#include "SessionManager.hpp"



void SessionManager::registerRoom(
    const std::string& roomId,
    Room* room)
{

    if(!room)
        return;


    m_rooms[roomId] = room;

}




Room*
SessionManager::getRoom(
    const std::string& roomId)
{

    auto it =
        m_rooms.find(roomId);



    if(it == m_rooms.end())
        return nullptr;



    return it->second;
}




const Room*
SessionManager::getRoom(
    const std::string& roomId) const
{

    auto it =
        m_rooms.find(roomId);



    if(it == m_rooms.end())
        return nullptr;



    return it->second;
}




bool SessionManager::exists(
    const std::string& roomId) const
{

    return
        m_rooms.find(roomId)
        !=
        m_rooms.end();

}