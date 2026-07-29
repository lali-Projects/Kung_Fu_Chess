#pragma once

#include <string>

#include "MoveResult.hpp"


class ClientSession;

namespace client
{
    class IClientCommandGateway;
}


class AuthenticationClient
{
public:
    AuthenticationClient(
        client::IClientCommandGateway& commandGateway,
        ClientSession& session);

    MoveResult registerUser(
        const std::string& username,
        const std::string& password);

    MoveResult login(
        const std::string& username,
        const std::string& password);

    MoveResult logout();

private:
    client::IClientCommandGateway& m_commandGateway;
    ClientSession& m_session;
};
