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


    static CommandType
    convertCommandType(
        const std::string& command);



    static bool
    validateArguments(
        CommandType type,
        const std::vector<std::string>& args);



    static bool
    validateClickArguments(
        const std::vector<std::string>& args);



    static bool
    isInteger(
        const std::string& value);

};