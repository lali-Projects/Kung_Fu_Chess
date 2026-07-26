#pragma once


#include <string>


#include "MoveResult.hpp"



class UserRepository;



/**
 * @brief Handles user authentication logic.
 *
 * Responsibilities:
 *
 *  - Register new users.
 *  - Authenticate existing users.
 *  - Validate authentication input.
 *
 *
 * Does NOT know:
 *
 *  - Database implementation.
 *  - SQL.
 *  - Network.
 *  - Server.
 *  - Game logic.
 */
class AuthService
{

public:


    explicit AuthService(
        UserRepository& repository);



public:


    MoveResult registerUser(
        const std::string& username,
        const std::string& password);



    MoveResult login(
        const std::string& username,
        const std::string& password);



private:


    bool validateUsername(
        const std::string& username) const;



    bool validatePassword(
        const std::string& password) const;



private:


    UserRepository& m_repository;

};