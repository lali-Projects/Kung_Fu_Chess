#include "AuthenticationClient.hpp"

#include "ClientSocket.hpp"

#include <sstream>




//================================================
// Constructor
//================================================

AuthenticationClient::AuthenticationClient(
    ClientSocket& socket)
:
m_socket(socket)
{
}





//================================================
// Register
//================================================

MoveResult AuthenticationClient::registerUser(
    const std::string& username,
    const std::string& password)
{

    std::ostringstream message;


    message
        << "REGISTER "
        << username
        << " "
        << password;



    bool sent =
        m_socket.send(
            message.str());



    if(!sent)
    {
        return
        {
            false,
            "send_failed"
        };
    }



    return
    {
        true,
        "register_request_sent"
    };

}








//================================================
// Login
//================================================

MoveResult AuthenticationClient::login(
    const std::string& username,
    const std::string& password)
{

    std::ostringstream message;


    message
        << "LOGIN "
        << username
        << " "
        << password;



    bool sent =
        m_socket.send(
            message.str());



    if(!sent)
    {
        return
        {
            false,
            "send_failed"
        };
    }



    return
    {
        true,
        "login_request_sent"
    };

}








//================================================
// Logout
//================================================

MoveResult AuthenticationClient::logout()
{

    bool sent =
        m_socket.send(
            "LOGOUT");



    if(!sent)
    {
        return
        {
            false,
            "send_failed"
        };
    }



    return
    {
        true,
        "logout_request_sent"
    };

}