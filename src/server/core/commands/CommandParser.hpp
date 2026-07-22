#pragma once

#include <optional>
#include <string>

#include "ClickCommand.hpp"


/**
 * @brief Converts external input into game commands.
 *
 * Current supported command:
 *
 *     CLICK row col
 *
 * Example:
 *
 *     CLICK 3 5
 *
 * becomes:
 *
 *     ClickCommand(Position(3,5))
 *
 *
 * Responsibilities:
 *  - Parse input.
 *  - Validate basic structure.
 *  - Create command objects.
 *
 * Does NOT:
 *  - Execute commands.
 *  - Know GameSession.
 *  - Know GameEngine.
 *  - Validate chess rules.
 */
class CommandParser
{
public:

    /**
     * @brief Parses a command string.
     *
     * @param input Raw command data.
     *
     * @return Parsed command or empty value
     *         if parsing failed.
     */
    std::optional<ClickCommand> parse(
        const std::string& input);



private:

    /**
     * @brief Parses CLICK command.
     */
    std::optional<ClickCommand> parseClick(
        const std::string& input);
};