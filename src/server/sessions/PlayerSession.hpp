#pragma once


#include <string>

#include "PieceTypes.hpp"



class PlayerSession
{

public:


    enum class ConnectionState
    {
        CONNECTED,
        DISCONNECTED
    };



public:


    explicit PlayerSession(
        std::string sessionId);



public:


//=================================
// Identity
//=================================

    const std::string&
    getSessionId() const;



    const std::string&
    getUserId() const;



    void setUserId(
        const std::string& id);



    const std::string&
    getUsername() const;



    void setUsername(
        const std::string& username);





//=================================
// Authentication
//=================================

    bool isAuthenticated() const;


    bool hasUser() const;



    void authenticate(
        const std::string& userId,
        const std::string& username);



    void logout();





//=================================
// Side
//=================================

    Side getSide() const;



    void setSide(
        Side side);



    void clearSide();






//=================================
// Connection
//=================================

    ConnectionState getState() const;



    void setState(
        ConnectionState state);



    bool isConnected() const;



    void disconnect();






//=================================
// Room
//=================================

    void setRoomId(
        const std::string& roomId);



    const std::string&
    getRoomId() const;



    bool hasRoom() const;



    void clearRoom();






//=================================
// Connection
//=================================

    void setConnectionId(
        int id);



    int getConnectionId() const;



public:


    void reset();



private:


    std::string m_sessionId;


    std::string m_userId;


    std::string m_username;



    bool m_authenticated{false};



    Side m_side{
        Side::NONE
    };



    ConnectionState m_state{
        ConnectionState::CONNECTED
    };



    std::string m_roomId;



    int m_connectionId{-1};

};