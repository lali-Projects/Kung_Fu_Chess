#pragma once

#include <optional>
#include <string>

#include "ClickCommand.hpp"


/**
 * @brief Converts external input into game commands.
 *
 * Responsibilities:
 *
 *  - Parse input syntax.
 *  - Create command objects.
 *
 * Does NOT:
 *
 *  - Execute commands.
 *  - Know GameSession.
 *  - Know GameEngine.
 *  - Validate chess rules.
 */
class CommandParser
{

public:

    std::optional<ClickCommand>
    parse(
        const std::string& input);



private:

    std::optional<ClickCommand>
    parseClick(
        const std::string& input);

};