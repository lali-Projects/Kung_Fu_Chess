#pragma once

#include <optional>
#include <string>

#include "User.hpp"

class IDatabase;

/**
 * @brief Repository for user persistence.
 *
 * Responsible only for:
 *  - Saving users.
 *  - Loading users.
 *  - Checking existence.
 */
class UserRepository
{
public:
    explicit UserRepository(IDatabase& database);

public:
    bool create(const User& user);

    std::optional<User> findByUsername(const std::string& username);

    bool exists(const std::string& username);

private:
    IDatabase& m_database;
};