#pragma once

#include <optional>
#include <string>
#include <vector>

#include "Command.hpp"

/**
 * @brief Parses raw client command strings into validated Command objects.
 *
 * Identifies the command type, extracts and validates arguments,
 * and converts CLICK coordinates into a valid board Position.
 */

class CommandParser
{
public:
    std::optional<Command> parse(const std::string& input);

private:
    static bool validateArguments(CommandType type, const std::vector<std::string>& args);
    static std::optional<Position> parseClickArguments(const std::vector<std::string>& args);
    static std::optional<int> parseBoardCoordinate(const std::string& value);
};
