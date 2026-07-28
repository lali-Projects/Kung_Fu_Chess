#pragma once

#include <string>

class User
{
public:
    User(
        std::string id,
        std::string username,
        std::string password
    );

    const std::string& getId() const;
    const std::string& getUsername() const;
    const std::string& getPassword() const;

private:
    std::string m_id;
    std::string m_username;
    std::string m_password;
};