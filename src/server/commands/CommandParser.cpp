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



    if(!(stream
            >> commandType
            >> row
            >> col))
    {
        return std::nullopt;
    }





    /*
        Parser responsibility only:

        - Validate syntax.
        - Create command.

        Does NOT:

        - Check board.
        - Check piece.
        - Check rules.
    */



    if(commandType != "CLICK")
    {
        return std::nullopt;
    }



    if(row < 0 || col < 0)
    {
        return std::nullopt;
    }



    return ClickCommand(
        Position(
            row,
            col));
}