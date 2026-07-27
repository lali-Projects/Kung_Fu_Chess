#pragma once


#include <string>
#include <memory>


#include "MoveResult.hpp"



class UserRepository;
class PlayerSession;



class AuthService
{

public:


    explicit AuthService(
        UserRepository& repository);



public:


    MoveResult registerUser(
        const std::string& username,
        const std::string& password);



    std::shared_ptr<PlayerSession> login(
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