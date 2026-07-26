#include "User.hpp"


#include <utility>



User::User(
    std::string id,
    std::string username,
    std::string password)
:
m_id(std::move(id)),
m_username(std::move(username)),
m_password(std::move(password))
{
}



const std::string&
User::getId() const
{
    return m_id;
}



const std::string&
User::getUsername() const
{
    return m_username;
}



const std::string&
User::getPassword() const
{
    return m_password;
}