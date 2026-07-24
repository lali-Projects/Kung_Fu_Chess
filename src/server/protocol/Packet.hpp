#pragma once

#include "PacketType.hpp"


/**
 * @brief Base class for all network packets.
 *
 * A Packet represents a unit of information
 * exchanged between client and server.
 *
 * This class only defines the common interface.
 *
 * It does NOT:
 *  - Execute commands.
 *  - Access game logic.
 *  - Know about sessions.
 *  - Know about players.
 */
class Packet
{
public:

    virtual ~Packet() = default;


    /**
     * @brief Returns the packet type.
     *
     * Used by the server to determine
     * how to process the packet.
     */
    virtual PacketType getType() const = 0;
};