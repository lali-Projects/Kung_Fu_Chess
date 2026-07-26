#pragma once


#include <string>
#include <utility>



/**
 * @brief Type of network message.
 *
 * Used only for routing.
 *
 * NetworkMessage does not interpret payload.
 */
enum class MessageType
{

    UNKNOWN,

    GAME_STATE,

    COMMAND,

    COMMAND_RESULT,

    SYSTEM_ERROR,

    DISCONNECT,

    HEARTBEAT

};





/**
 * @brief Transport independent message container.
 *
 * Responsibilities:
 *
 *  - Store message type.
 *  - Store raw payload.
 *  - Provide message state checks.
 *
 *
 * Does NOT know:
 *
 *  - JSON
 *  - WebSocket
 *  - TCP
 *  - Game logic
 *  - Authentication
 */
class NetworkMessage
{

public:


    NetworkMessage();


    NetworkMessage(
        MessageType type,
        const std::string& payload);



    NetworkMessage(
        MessageType type,
        std::string&& payload);



    ~NetworkMessage() = default;



public:

    NetworkMessage(
        const NetworkMessage&) = default;


    NetworkMessage& operator=(
        const NetworkMessage&) = default;



    NetworkMessage(
        NetworkMessage&&) noexcept = default;


    NetworkMessage& operator=(
        NetworkMessage&&) noexcept = default;



public:


    //---------------------------------
    // Type
    //---------------------------------

    void setType(
        MessageType type);



    MessageType getType() const;



    std::string typeName() const;



public:


    //---------------------------------
    // Payload
    //---------------------------------

    void setPayload(
        const std::string& payload);



    void setPayload(
        std::string&& payload);



    const std::string&
    getPayload() const;



public:


    //---------------------------------
    // State
    //---------------------------------

    bool empty() const;


    bool valid() const;



    void clear();



public:


    //---------------------------------
    // Type helpers
    //---------------------------------

    bool isCommand() const;



    bool isGameState() const;



    bool isError() const;



    bool isDisconnect() const;



    bool isHeartbeat() const;



private:


    MessageType m_type{
        MessageType::UNKNOWN
    };


    std::string m_payload;

};