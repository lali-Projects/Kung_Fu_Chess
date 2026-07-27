#include "ConsoleClient.hpp"


#include "AuthenticationClient.hpp"
#include "ClientSocket.hpp"


#include <iostream>




//================================================
// Constructor
//================================================

ConsoleClient::ConsoleClient(
    ClientSocket& socket,
    AuthenticationClient& authentication)
:
m_socket(socket),
m_authentication(authentication)
{
}
//================================================
// Run
//================================================

void ConsoleClient::run()
{

    while(m_running)
    {

        printMenu();


        int choice;

        std::cin >> choice;



        switch(choice)
        {

            case 1:
                handleRegister();
                break;



            case 2:
                handleLogin();
                break;



            case 3:
                handleLogout();
                break;



            case 4:
                m_running = false;
                break;



            default:

                std::cout
                    << "Invalid option\n";

                break;
        }

    }

}







//================================================
// Menu
//================================================

void ConsoleClient::printMenu()
{

    std::cout
        << "\n========== CLIENT ==========\n"
        << "1. Register\n"
        << "2. Login\n"
        << "3. Logout\n"
        << "4. Exit\n"
        << "Select: ";

}
//================================================
// Register
//================================================

void ConsoleClient::handleRegister()
{

    std::string username;

    std::string password;



    std::cout
        << "Username: ";

    std::cin
        >> username;



    std::cout
        << "Password: ";

    std::cin
        >> password;



    MoveResult result =
        m_authentication.registerUser(
            username,
            password);



    std::cout
        << result.reason
        << "\n";

}
//================================================
// Login
//================================================

void ConsoleClient::handleLogin()
{

    std::string username;

    std::string password;



    std::cout
        << "Username: ";

    std::cin
        >> username;



    std::cout
        << "Password: ";

    std::cin
        >> password;



    MoveResult result =
        m_authentication.login(
            username,
            password);



    std::cout
        << result.reason
        << "\n";

}
//================================================
// Logout
//================================================

void ConsoleClient::handleLogout()
{

    MoveResult result =
        m_authentication.logout();



    std::cout
        << result.reason
        << "\n";

}