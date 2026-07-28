#pragma once


#include <optional>
#include <string>


#include "Command.hpp"



class CommandParser
{

public:


    std::optional<Command>
    parse(
        const std::string& input);



private:


    static bool
    validateArguments(
        CommandType type,
        const std::vector<std::string>& args);



    static std::optional<Position>
    parseClickArguments(
        const std::vector<std::string>& args);



    static std::optional<int>
    parseBoardCoordinate(
        const std::string& value);

};
