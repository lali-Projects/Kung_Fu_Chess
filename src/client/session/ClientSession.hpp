#pragma once

#include <string>


/**
 * @brief Stores client-side user session state.
 *
 * Responsible only for maintaining
 * the current client session.
 *
 * Does NOT know:
 *
 *  - Network
 *  - Server
 *  - GUI
 *  - Game rules
 */
class ClientSession
{

public:

    ClientSession() = default;


public:

    //=================================
    // Authentication
    //=================================

    void login(
        const std::string& userId,
        const std::string& username,
        const std::string& sessionId);



    void logout();



    bool isAuthenticated() const;



public:

    //=================================
    // Identity
    //=================================

    const std::string&
    getUserId() const;



    const std::string&
    getUsername() const;



    const std::string&
    getSessionId() const;



public:

    //=================================
    // Room
    //=================================

    void setRoomId(
        const std::string& roomId);



    void clearRoom();



    bool hasRoom() const;



    const std::string&
    getRoomId() const;



public:

    void reset();



private:

    std::string m_userId;

    std::string m_username;

    std::string m_sessionId;


    std::string m_roomId;


    bool m_authenticated{false};

};