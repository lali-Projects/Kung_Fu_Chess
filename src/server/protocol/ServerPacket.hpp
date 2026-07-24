#pragma once

#include "Packet.hpp"


/**
 * @brief Base class for packets
 * sent from server to clients.
 *
 * ServerPacket represents
 * outgoing communication.
 *
 * Does NOT:
 *  - Manage connections.
 *  - Send data directly.
 *  - Know networking details.
 */
class ServerPacket : public Packet
{
public:

    virtual ~ServerPacket() = default;
};