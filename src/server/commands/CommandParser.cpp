#include "CommandParser.hpp"


#include <cctype>
#include <sstream>



std::optional<Command>
CommandParser::parse(
    const std::string& input)
{

    if(input.empty())
        return std::nullopt;



    std::stringstream stream(input);



    std::string commandName;

    stream >> commandName;



    CommandType type =
        convertCommandType(commandName);



    if(type == CommandType::UNKNOWN)
        return std::nullopt;




    std::vector<std::string> args;



    std::string value;


    while(stream >> value)
    {
        args.push_back(value);
    }





    if(!validateArguments(
            type,
            args))
    {
        return std::nullopt;
    }






    return Command(
        type,
        args);

}









//================================================
// Validate Arguments
//================================================

bool CommandParser::validateArguments(
    CommandType type,
    const std::vector<std::string>& args)
{

    switch(type)
    {

        case CommandType::CLICK:

            return
                validateClickArguments(args);



        case CommandType::LOGIN:

        case CommandType::REGISTER:

            return
                args.size() == 2;



        case CommandType::CREATE_ROOM:

        case CommandType::JOIN_ROOM:

            return
                args.size() == 1;


        case CommandType::LEAVE_ROOM:

        case CommandType::LOGOUT:

            return
                args.empty();



        default:

            return false;
    }

}









//================================================
// Validate CLICK
//================================================

bool CommandParser::validateClickArguments(
    const std::vector<std::string>& args)
{

    if(args.size() != 2)
        return false;



    if(!isInteger(args[0]) ||
       !isInteger(args[1]))
    {
        return false;
    }





    int row;
    int col;


    try
    {
        row = std::stoi(args[0]);
        col = std::stoi(args[1]);
    }
    catch(...)
    {
        return false;
    }





    return
        row >= 0 &&
        row < 8 &&
        col >= 0 &&
        col < 8;

}









//================================================
// Integer Check
//================================================

bool CommandParser::isInteger(
    const std::string& value)
{

    if(value.empty())
        return false;



    for(char c : value)
    {
        if(!std::isdigit(c))
            return false;
    }



    return true;

}









//================================================
// Convert Command
//================================================

CommandType
CommandParser::convertCommandType(
    const std::string& command)
{

    if(command == "CLICK")
        return CommandType::CLICK;



    if(command == "LOGIN")
        return CommandType::LOGIN;



    if(command == "REGISTER")
        return CommandType::REGISTER;



    if(command == "CREATE_ROOM")
        return CommandType::CREATE_ROOM;



    if(command == "JOIN_ROOM")
        return CommandType::JOIN_ROOM;


    if(command == "LEAVE_ROOM")
        return CommandType::LEAVE_ROOM;


    if(command == "LOGOUT")
        return CommandType::LOGOUT;



    return CommandType::UNKNOWN;

}
