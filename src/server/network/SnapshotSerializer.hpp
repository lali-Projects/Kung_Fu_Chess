#pragma once


#include <string>

#include "GameSnapshot.hpp"
#include "NetworkMessage.hpp"



/**
 * @brief Converts game snapshots into network messages.
 *
 * Responsibilities:
 *
 *  - Serialize GameSnapshot.
 *  - Create JSON representation.
 *  - Prepare data for network transfer.
 *
 *
 * Does NOT know:
 *
 *  - Server.
 *  - Clients.
 *  - GameEngine.
 *  - Rules.
 */
class SnapshotSerializer
{

public:


    /**
     * @brief Converts snapshot to JSON network message.
     */
    static NetworkMessage serialize(
        const GameSnapshot& snapshot);



private:


    /**
     * @brief Converts snapshot into JSON string.
     */
    static std::string toJson(
        const GameSnapshot& snapshot);

};