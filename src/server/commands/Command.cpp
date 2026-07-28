#include "Command.hpp"

#include <array>

namespace
{
    struct CommandTypeName
    {
        CommandType type;
        const char* name;
    };

    constexpr std::array<CommandTypeName, 8> COMMAND_TYPE_NAMES{{
        {CommandType::UNKNOWN, "UNKNOWN"},
        {CommandType::CLICK, "CLICK"},
        {CommandType::LOGIN, "LOGIN"},
        {CommandType::REGISTER, "REGISTER"},
        {CommandType::CREATE_ROOM, "CREATE_ROOM"},
        {CommandType::JOIN_ROOM, "JOIN_ROOM"},
        {CommandType::LEAVE_ROOM, "LEAVE_ROOM"},
        {CommandType::LOGOUT, "LOGOUT"}
    }};
}

CommandType commandTypeFromString(const std::string& command)
{
    for (const auto& entry : COMMAND_TYPE_NAMES)
    {
        if (command == entry.name)
        {
            return entry.type;
        }
    }

    return COMMAND_TYPE_NAMES.front().type;
}

std::string commandTypeToString(CommandType type)
{
    for (const auto& entry : COMMAND_TYPE_NAMES)
    {
        if (type == entry.type)
        {
            return entry.name;
        }
    }

    return COMMAND_TYPE_NAMES.front().name;
}