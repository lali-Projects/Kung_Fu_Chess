#pragma once

#include <string>

#include "PieceTypes.hpp"


/**
 * @brief Represents a connected player.
 *
 * Belongs to server core layer.
 *
 * Responsibilities:
 *
 *  - Store player identity.
 *  - Store connection state.
 *  - Store assigned game side.
 *  - Store room association.
 *  - Store connection identifier.
 *
 *
 * Does NOT know:
 *
 *  - GameEngine.
 *  - Board.
 *  - Rules.
 *  - Network transport.
 *  - WebSocket.
 */
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
        std::string id);



public:


    const std::string& getId() const;



    Side getSide() const;


    void setSide(
        Side side);



    ConnectionState getState() const;


    void setState(
        ConnectionState state);



public:

    //==============================
    // Room association
    //==============================


    void setRoomId(
        const std::string& roomId);



    const std::string& getRoomId() const;



    bool hasRoom() const;



public:

    //==============================
    // Connection identification
    //==============================


    void setConnectionId(
        int id);



    int getConnectionId() const;

void clearRoom();

private:


    std::string m_id;



    Side m_side{
        Side::WHITE
    };



    ConnectionState m_state{
        ConnectionState::CONNECTED
    };



    /*
        Room that owns this player.
    */
    std::string m_roomId;



    /*
        Logical connection identifier.

        Does not represent transport.
    */
    int m_connectionId{-1};

};