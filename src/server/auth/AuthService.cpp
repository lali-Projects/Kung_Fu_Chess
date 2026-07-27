#include "AuthService.hpp"


#include "UserRepository.hpp"
#include "User.hpp"
#include "PlayerSession.hpp"


#include <utility>





//================================================
// Constructor
//================================================

AuthService::AuthService(
    UserRepository& repository)
:
m_repository(repository)
{
}







//================================================
// Register User
//================================================

MoveResult AuthService::registerUser(
    const std::string& username,
    const std::string& password)
{

    if(!validateUsername(username))
    {
        return
        {
            false,
            "invalid_username"
        };
    }



    if(!validatePassword(password))
    {
        return
        {
            false,
            "invalid_password"
        };
    }





    if(m_repository.exists(username))
    {
        return
        {
            false,
            "username_exists"
        };
    }






    User user(
        "",
        username,
        password);






    if(!m_repository.create(user))
    {
        return
        {
            false,
            "user_creation_failed"
        };
    }







    return
    {
        true,
        "register_success"
    };

}









//================================================
// Login
//================================================

std::shared_ptr<PlayerSession>
AuthService::login(
    const std::string& username,
    const std::string& password)
{

    if(!validateUsername(username))
    {
        return nullptr;
    }




    if(!validatePassword(password))
    {
        return nullptr;
    }







    auto user =
        m_repository.findByUsername(
            username);





    if(!user)
    {
        return nullptr;
    }







    if(user->getPassword()
       !=
       password)
    {
        return nullptr;
    }







    auto player =
        std::make_shared<PlayerSession>(
            username);





    return player;

}









//================================================
// Username Validation
//================================================

bool AuthService::validateUsername(
    const std::string& username) const
{

    if(username.size() < 3)
        return false;


    if(username.size() > 20)
        return false;



    return true;

}









//================================================
// Password Validation
//================================================

bool AuthService::validatePassword(
    const std::string& password) const
{

    if(password.size() < 4)
        return false;


    if(password.size() > 64)
        return false;



    return true;

}