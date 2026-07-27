#include "AuthenticationClient.hpp"

#include <sstream>

#include "ClientSession.hpp"
#include "CommandResponse.hpp"
#include "IClientCommandGateway.hpp"


AuthenticationClient::AuthenticationClient(
    client::IClientCommandGateway& commandGateway,
    ClientSession& session)
    : m_commandGateway(commandGateway),
      m_session(session)
{
}


MoveResult AuthenticationClient::registerUser(
    const std::string& username,
    const std::string& password)
{
    std::ostringstream command;
    command
        << "REGISTER "
        << username
        << " "
        << password;

    const CommandResponse response =
        m_commandGateway.sendCommandAndWait(
            command.str());

    return {response.success, response.reason};
}


MoveResult AuthenticationClient::login(
    const std::string& username,
    const std::string& password)
{
    std::ostringstream command;
    command
        << "LOGIN "
        << username
        << " "
        << password;

    const CommandResponse response =
        m_commandGateway.sendCommandAndWait(
            command.str());

    if(response.success &&
       response.userId &&
       response.username &&
       response.sessionId)
    {
        m_session.login(
            *response.userId,
            *response.username,
            *response.sessionId);
    }

    if(response.success &&
       (!response.userId ||
        !response.username ||
        !response.sessionId))
    {
        return {false, "invalid_login_response"};
    }

    return {response.success, response.reason};
}


MoveResult AuthenticationClient::logout()
{
    const CommandResponse response =
        m_commandGateway.sendCommandAndWait(
            "LOGOUT");

    if(response.success)
    {
        m_session.logout();
    }

    return {response.success, response.reason};
}
