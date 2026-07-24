#include "NetworkMessage.hpp"



//================================================
// Default Constructor
//================================================

NetworkMessage::NetworkMessage()
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
// Set Payload
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



//================================================
// Get Payload
//================================================

const std::string&
NetworkMessage::getPayload() const
{
    return m_payload;
}



//================================================
// Empty
//================================================

bool NetworkMessage::empty() const
{
    return m_payload.empty();
}



//================================================
// Valid
//================================================

bool NetworkMessage::valid() const
{
    return
        m_type != MessageType::UNKNOWN
        &&
        !m_payload.empty();
}



//================================================
// Clear
//================================================

void NetworkMessage::clear()
{

    m_type =
        MessageType::UNKNOWN;


    m_payload.clear();

}