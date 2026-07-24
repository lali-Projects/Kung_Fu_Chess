#include "ClickPacket.hpp"


//================================================
// Constructor
//================================================

ClickPacket::ClickPacket(
    const Position& position)
    :
    m_position(position)
{
}


//================================================
// Packet Type
//================================================

PacketType ClickPacket::getType() const
{
    return PacketType::CLICK;
}


//================================================
// Position
//================================================

const Position& ClickPacket::getPosition() const
{
    return m_position;
}