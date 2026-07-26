#include "AuthService.hpp"


#include "UserRepository.hpp"
#include "User.hpp"



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

    //---------------------------------
    // Validate username
    //---------------------------------

    if(!validateUsername(username))
    {
        return
        {
            false,
            "invalid_username"
        };
    }



    //---------------------------------
    // Validate password
    //---------------------------------

    if(!validatePassword(password))
    {
        return
        {
            false,
            "invalid_password"
        };
    }




    //---------------------------------
    // Check existing user
    //---------------------------------

    if(m_repository.exists(username))
    {
        return
        {
            false,
            "username_exists"
        };
    }





    //---------------------------------
    // Create user
    //---------------------------------

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







    //---------------------------------
    // Verify creation
    //---------------------------------

    auto createdUser =
        m_repository.findByUsername(
            username);



    if(!createdUser)
    {
        return
        {
            false,
            "user_not_found_after_creation"
        };
    }







    return
    {
        true,
        "register_success"
    };

}









//================================================
// Login User
//================================================

MoveResult AuthService::login(
    const std::string& username,
    const std::string& password)
{

    //---------------------------------
    // Validate username
    //---------------------------------

    if(!validateUsername(username))
    {
        return
        {
            false,
            "invalid_username"
        };
    }




    //---------------------------------
    // Validate password
    //---------------------------------

    if(!validatePassword(password))
    {
        return
        {
            false,
            "invalid_password"
        };
    }






    //---------------------------------
    // Find user
    //---------------------------------

    auto user =
        m_repository.findByUsername(
            username);





    if(!user)
    {
        return
        {
            false,
            "user_not_found"
        };
    }







    //---------------------------------
    // Check password
    //---------------------------------

    if(user->getPassword()
        !=
       password)
    {
        return
        {
            false,
            "wrong_password"
        };
    }






    return
    {
        true,
        "login_success"
    };

}









//================================================
// Username Validation
//================================================

bool AuthService::validateUsername(
    const std::string& username) const
{

    return
        username.size() >= 3
        &&
        username.size() <= 20;

}









//================================================
// Password Validation
//================================================

bool AuthService::validatePassword(
    const std::string& password) const
{

    return
        password.size() >= 4
        &&
        password.size() <= 64;

}