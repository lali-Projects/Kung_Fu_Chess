#pragma once


#include <string>
#include <vector>
#include <utility>





enum class CommandType
{
    UNKNOWN,

    CLICK,

    LOGIN,

    REGISTER,

    CREATE_ROOM,

    JOIN_ROOM
};







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






    CommandType getType() const
    {
        return m_type;
    }







    const std::vector<std::string>&
    getArgs() const
    {
        return m_args;
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

};