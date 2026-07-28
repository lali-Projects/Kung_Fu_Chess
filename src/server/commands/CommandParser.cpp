#include "CommandParser.hpp"


#include <charconv>
#include <sstream>
#include <system_error>



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
        commandTypeFromString(commandName);



    if(type == CommandType::UNKNOWN)
        return std::nullopt;




    std::vector<std::string> args;



    std::string value;


    while(stream >> value)
    {
        args.push_back(value);
    }





    if(type == CommandType::CLICK)
    {
        const auto clickPosition =
            parseClickArguments(args);


        if(!clickPosition)
        {
            return std::nullopt;
        }


        return Command(
            type,
            args,
            *clickPosition);
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
// Parse CLICK
//================================================

std::optional<Position>
CommandParser::parseClickArguments(
    const std::vector<std::string>& args)
{

    if(args.size() != 2)
        return std::nullopt;



    const auto row =
        parseBoardCoordinate(args[0]);


    const auto col =
        parseBoardCoordinate(args[1]);


    if(!row || !col)
    {
        return std::nullopt;
    }


    return Position(*row, *col);

}









//================================================
// Coordinate Parsing
//================================================

std::optional<int>
CommandParser::parseBoardCoordinate(
    const std::string& value)
{

    if(value.empty())
        return std::nullopt;



    for(char c : value)
    {
        if(c < '0' || c > '9')
            return std::nullopt;
    }



    int coordinate = 0;
    const char* begin = value.data();
    const char* end = begin + value.size();
    const auto result =
        std::from_chars(
            begin,
            end,
            coordinate);


    if(result.ec != std::errc{} ||
       result.ptr != end ||
       coordinate < 0 ||
       coordinate >= 8)
    {
        return std::nullopt;
    }


    return coordinate;

}
