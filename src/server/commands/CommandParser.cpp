#include "CommandParser.hpp"

#include <sstream>


//================================================
// Parse Command
//================================================

std::optional<ClickCommand>
CommandParser::parse(
    const std::string& input)
{
    std::stringstream stream(input);

    std::string commandType;

    stream >> commandType;


    if(commandType == "CLICK")
    {
        return parseClick(input);
    }


    return std::nullopt;
}



//================================================
// Parse CLICK
//================================================

std::optional<ClickCommand>
CommandParser::parseClick(
    const std::string& input)
{
    std::stringstream stream(input);


    std::string commandType;

    int row;
    int col;



    stream
        >> commandType
        >> row
        >> col;



    /*
        Basic validation only.

        Board rules remain
        inside GameController/GameEngine.
    */

    if(commandType != "CLICK")
    {
        return std::nullopt;
    }



    if(row < 0 || col < 0)
    {
        return std::nullopt;
    }



    Position position(
        row,
        col);



    return ClickCommand(
        position);
}