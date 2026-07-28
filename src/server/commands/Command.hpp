#pragma once


#include <string>
#include <optional>
#include <vector>
#include <utility>

#include "Position.hpp"




enum class CommandType
{
    UNKNOWN,

    CLICK,

    LOGIN,

    REGISTER,

    CREATE_ROOM,

    JOIN_ROOM,

    LEAVE_ROOM,

    LOGOUT
};


CommandType commandTypeFromString(
    const std::string& command);


std::string commandTypeToString(
    CommandType type);







class Command
{

public:


    Command(
        CommandType type,
        std::vector<std::string> args)
    :
    m_type(type),
    m_args(std::move(args))
    {
    }



    Command(
        CommandType type,
        std::vector<std::string> args,
        const Position& clickPosition)
    :
    m_type(type),
    m_args(std::move(args)),
    m_clickPosition(clickPosition)
    {
    }






    CommandType getType() const
    {
        return m_type;
    }







    const std::vector<std::string>&
    getArgs() const
    {
        return m_args;
    }



    const Position&
    getClickPosition() const
    {
        return m_clickPosition.value();
    }






    bool isValid() const
    {
        return
            m_type != CommandType::UNKNOWN;
    }







private:


    CommandType m_type{
        CommandType::UNKNOWN
    };



    std::vector<std::string> m_args;


    std::optional<Position> m_clickPosition;

};
