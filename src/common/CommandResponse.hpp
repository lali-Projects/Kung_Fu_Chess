#pragma once

#include <optional>
#include <string>

#include "PieceTypes.hpp"
#include "Position.hpp"


/**
 * @brief Transport DTO for the result of a text command.
 *
 * The command request grammar remains text based. This DTO only gives
 * command replies a stable, shared JSON representation that both the
 * server and client can understand.
 */
struct CommandResponse
{
    bool success{false};

    std::string reason;

    std::string command;

    std::optional<std::string> userId;

    std::optional<std::string> username;

    std::optional<std::string> sessionId;

    std::optional<std::string> roomId;

    std::optional<Side> side;

    /**
     * Per-player presentation selection returned with an explicit CLICK
     * result. A missing value on such a result means clear selection.
     */
    std::optional<Position> selectedPosition;
};
