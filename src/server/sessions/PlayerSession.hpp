#pragma once


#include <mutex>
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

    std::string
    getSessionId() const;



    std::string
    getUserId() const;



    std::string
    getUsername() const;





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

    ConnectionState
    getState() const;



    void setState(
        ConnectionState state);



    bool isConnected() const;



    void disconnect();







//=================================
// Room
//=================================

    void setRoomId(
        const std::string& roomId);



    std::string
    getRoomId() const;



    bool hasRoom() const;



    void clearRoom();



    void leaveGame();



    bool isInGame() const;







//=================================
// Connection ID
//=================================

    void setConnectionId(
        int id);



    int getConnectionId() const;







//=================================
// State Helpers
//=================================

    bool canJoinRoom() const;



    void reset();






private:


    mutable std::recursive_mutex m_mutex;


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
