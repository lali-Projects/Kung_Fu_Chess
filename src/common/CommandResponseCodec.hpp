#pragma once

#include <optional>
#include <string>

#include "CommandResponse.hpp"


/**
 * @brief JSON codec for CommandResponse payloads.
 *
 * This codec operates on the payload only. NetworkMessage remains
 * responsible for the existing TYPE|payload wire envelope.
 */
class CommandResponseCodec
{
public:
    static std::string serialize(
        const CommandResponse& response);

    /**
     * @return Parsed response, or std::nullopt for malformed/invalid JSON.
     * This function does not propagate JSON conversion exceptions.
     */
    static std::optional<CommandResponse> deserialize(
        const std::string& payload);
};
