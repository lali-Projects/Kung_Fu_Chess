#pragma once

#include "Packet.hpp"


/**
 * @brief Base class for packets
 * received from clients.
 *
 * ClientPacket represents
 * incoming communication.
 *
 * Does NOT:
 *  - Execute commands.
 *  - Access GameEngine.
 *  - Modify game state.
 */
class ClientPacket : public Packet
{
public:

    virtual ~ClientPacket() = default;
};