#pragma once


#include <string>
#include <unordered_map>



class Room;



/**
 * @brief Provides access to active room sessions.
 *
 * Temporary bridge while Server migrates
 * from single session architecture
 * to RoomManager architecture.
 *
 *
 * Does NOT know:
 *
 * - Game rules.
 * - Players.
 * - Network.
 */
class SessionManager
{

public:


    SessionManager() = default;


    ~SessionManager() = default;



    SessionManager(
        const SessionManager&) = delete;



    SessionManager& operator=(
        const SessionManager&) = delete;



public:


    void registerRoom(
        const std::string& roomId,
        Room* room);



    Room* getRoom(
        const std::string& roomId);



    const Room* getRoom(
        const std::string& roomId) const;



    bool exists(
        const std::string& roomId) const;



private:


    std::unordered_map<
        std::string,
        Room*> m_rooms;

};