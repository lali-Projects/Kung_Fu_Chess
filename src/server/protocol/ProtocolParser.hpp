#pragma once


#include <optional>
#include <memory>


#include "ClientPacket.hpp"
#include "ClickCommand.hpp"



/**
 * @brief Converts protocol packets into
 * internal game commands.
 *
 * Responsibilities:
 *
 *  - Read packet type.
 *  - Convert packet data.
 *  - Create command objects.
 *
 * Does NOT:
 *
 *  - Execute commands.
 *  - Know GameSession.
 *  - Know GameEngine.
 *  - Validate game rules.
 */
class ProtocolParser
{
public:

    /**
     * @brief Converts client packet
     * into game command.
     *
     * @param packet Incoming client packet.
     *
     * @return Command or empty value
     *         if packet cannot be converted.
     */
    std::optional<ClickCommand> parse(
        const ClientPacket& packet);



private:

    /**
     * @brief Parses CLICK packet.
     */
    std::optional<ClickCommand> parseClick(
        const ClientPacket& packet);
};