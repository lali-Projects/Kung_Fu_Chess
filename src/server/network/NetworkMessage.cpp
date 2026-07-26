#include "NetworkMessage.hpp"



//================================================
// Default Constructor
//================================================

NetworkMessage::NetworkMessage()
:
m_type(MessageType::UNKNOWN),
m_payload()
{
}






//================================================
// Constructor
//================================================

NetworkMessage::NetworkMessage(
    MessageType type,
    const std::string& payload)
:
m_type(type),
m_payload(payload)
{
}






//================================================
// Move Constructor
//================================================

NetworkMessage::NetworkMessage(
    MessageType type,
    std::string&& payload)
:
m_type(type),
m_payload(std::move(payload))
{
}






//================================================
// Set Type
//================================================

void NetworkMessage::setType(
    MessageType type)
{
    m_type = type;
}






//================================================
// Get Type
//================================================

MessageType NetworkMessage::getType() const
{
    return m_type;
}






//================================================
// Type Name
//================================================

std::string NetworkMessage::typeName() const
{

    switch(m_type)
    {

        case MessageType::GAME_STATE:
            return "GAME_STATE";


        case MessageType::COMMAND:
            return "COMMAND";


        case MessageType::COMMAND_RESULT:
            return "COMMAND_RESULT";


        case MessageType::SYSTEM_ERROR:
            return "SYSTEM_ERROR";


        case MessageType::DISCONNECT:
            return "DISCONNECT";


        case MessageType::HEARTBEAT:
            return "HEARTBEAT";


        default:
            return "UNKNOWN";
    }

}






//================================================
// Payload
//================================================

void NetworkMessage::setPayload(
    const std::string& payload)
{
    m_payload = payload;
}






void NetworkMessage::setPayload(
    std::string&& payload)
{
    m_payload = std::move(payload);
}






const std::string&
NetworkMessage::getPayload() const
{
    return m_payload;
}






//================================================
// State
//================================================

bool NetworkMessage::empty() const
{
    return m_payload.empty();
}






bool NetworkMessage::valid() const
{
    return
        m_type != MessageType::UNKNOWN
        &&
        !m_payload.empty();
}






void NetworkMessage::clear()
{

    m_type =
        MessageType::UNKNOWN;


    m_payload.clear();

}






//================================================
// Helpers
//================================================

bool NetworkMessage::isCommand() const
{
    return
        m_type == MessageType::COMMAND;
}






bool NetworkMessage::isGameState() const
{
    return
        m_type == MessageType::GAME_STATE;
}






bool NetworkMessage::isError() const
{
    return
        m_type == MessageType::SYSTEM_ERROR;
}






bool NetworkMessage::isDisconnect() const
{
    return
        m_type == MessageType::DISCONNECT;
}






bool NetworkMessage::isHeartbeat() const
{
    return
        m_type == MessageType::HEARTBEAT;
}