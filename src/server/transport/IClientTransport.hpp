#pragma once

#include "GameSnapshot.hpp"

/**
 * @brief Abstract communication channel.
 *
 * Server sends snapshots through this interface.
 *
 * Current implementation:
 *      Local memory.
 *
 * Future implementation:
 *      WebSocket.
 */
class IClientTransport
{
public:

    virtual ~IClientTransport() = default;

    /**
     * @brief Sends snapshot to client.
     */
    virtual void sendSnapshot(
        const GameSnapshot& snapshot) = 0;
};