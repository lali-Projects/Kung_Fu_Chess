#pragma once

#include <string>

#include "MoveResult.hpp"


class ClientSocket;


/**
 * @brief Client side authentication service.
 *
 * Responsible for:
 *
 *  - Sending register requests.
 *  - Sending login requests.
 *
 *
 * Does NOT know:
 *
 *  - GUI.
 *  - Game logic.
 *  - Server implementation.
 *  - Database.
 */
class AuthenticationClient
{

public:

    explicit AuthenticationClient(
        ClientSocket& socket);



public:


    MoveResult registerUser(
        const std::string& username,
        const std::string& password);



    MoveResult login(
        const std::string& username,
        const std::string& password);



    MoveResult logout();



private:


    ClientSocket& m_socket;

};