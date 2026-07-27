#pragma once

#include <optional>
#include <string>

#include "GameSnapshot.hpp"
#include "NetworkMessage.hpp"


/**
 * @brief Reconstructs the existing GUI snapshot model from wire JSON.
 */
class SnapshotDeserializer
{
public:
    /**
     * Accepts only GAME_STATE messages.
     */
    static std::optional<GameSnapshot> deserialize(
        const NetworkMessage& message);

    /**
     * Parses the JSON payload without the TYPE| envelope.
     */
    static std::optional<GameSnapshot> deserializePayload(
        const std::string& payload);
};
