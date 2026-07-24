#pragma once


#include <string>
#include <utility>





/**
 * @brief Type of network message.
 *
 * Used only for routing.
 *
 * NetworkMessage does not interpret
 * payload content.
 */
enum class MessageType
{

    UNKNOWN,

    GAME_STATE,

    COMMAND,

    ERROR,

    DISCONNECT,

    HEARTBEAT

};





/**
 * @brief Represents a transport independent message.
 *
 *
 * Responsibilities:
 *
 *  - Store message type.
 *  - Store raw payload.
 *  - Provide common message container.
 *
 *
 * Does NOT know:
 *
 *  - JSON.
 *  - WebSocket.
 *  - TCP.
 *  - Game logic.
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




    //---------------------------------
    // Payload
    //---------------------------------

    void setPayload(
        const std::string& payload);



    void setPayload(
        std::string&& payload);



    const std::string&
    getPayload() const;




    //---------------------------------
    // State
    //---------------------------------

    bool empty() const;



    bool valid() const;



    void clear();




private:


    MessageType m_type{
        MessageType::UNKNOWN
    };



    std::string m_payload;

};