#pragma once

#include <memory>


class AuthenticationClient;
class ClientSocket;


/**
 * @brief Console test client.
 *
 * Used to verify server communication
 * before GUI integration.
 */
class ConsoleClient
{

public:

    ConsoleClient(
        ClientSocket& socket,
        AuthenticationClient& authentication);


public:

    void run();


private:

    void printMenu();


    void handleRegister();


    void handleLogin();


    void handleLogout();



private:

    ClientSocket& m_socket;

    AuthenticationClient& m_authentication;

    bool m_running{true};

};