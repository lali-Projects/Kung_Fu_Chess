#include "ClientApplicationController.hpp"

#include <optional>

#include "AuthenticationClient.hpp"
#include "AuthoritativeGameState.hpp"
#include "ClientSession.hpp"
#include "ITransport.hpp"
#include "RoomClient.hpp"


ClientApplicationController::ClientApplicationController(
    client::ITransport& transport,
    AuthenticationClient& authentication,
    RoomClient& rooms,
    ClientSession& session,
    AuthoritativeGameState& gameState)
    : m_transport(transport),
      m_authentication(authentication),
      m_rooms(rooms),
      m_session(session),
      m_gameState(gameState)
{
}


MoveResult ClientApplicationController::connectServer(
    const std::string& url)
{
    if(url.empty())
    {
        return {false, "empty_server_url"};
    }
    if(m_transport.isConnected())
    {
        return {true, "already_connected"};
    }

    const bool connected = m_transport.connect(url);
    return {
        connected,
        connected ? "connected" : "connection_failed"
    };
}


void ClientApplicationController::disconnect()
{
    m_transport.disconnect();
}


MoveResult ClientApplicationController::registerUser(
    const std::string& username,
    const std::string& password)
{
    if(!isConnected())
    {
        return {false, "not_connected"};
    }
    if(username.empty() || password.empty())
    {
        return {false, "credentials_required"};
    }
    return m_authentication.registerUser(username, password);
}


MoveResult ClientApplicationController::login(
    const std::string& username,
    const std::string& password)
{
    if(!isConnected())
    {
        return {false, "not_connected"};
    }
    if(username.empty() || password.empty())
    {
        return {false, "credentials_required"};
    }
    return m_authentication.login(username, password);
}


MoveResult ClientApplicationController::logout()
{
    if(!isConnected())
    {
        return {false, "not_connected"};
    }

    const MoveResult result = m_authentication.logout();
    if(result.success)
    {
        m_gameState.clear();
    }
    return result;
}


MoveResult ClientApplicationController::createRoom(
    const std::string& roomId)
{
    if(!isAuthenticated())
    {
        return {false, "not_authenticated"};
    }
    if(roomId.empty())
    {
        return {false, "room_id_required"};
    }
    return m_rooms.createRoom(roomId);
}


MoveResult ClientApplicationController::joinRoom(
    const std::string& roomId)
{
    if(!isAuthenticated())
    {
        return {false, "not_authenticated"};
    }
    if(isInRoom())
    {
        return {false, "already_in_room"};
    }
    if(roomId.empty())
    {
        return {false, "room_id_required"};
    }

    // Do this before the request: the server may send the first snapshot
    // immediately before its JOIN result, and clearing afterwards would lose it.
    m_gameState.clear();
    const MoveResult result =
        m_rooms.joinRoom(roomId);

    if(result.success &&
       m_session.getSide() == Side::OBSERVER)
    {
        // A spectator never owns an authoritative gameplay selection. Keep
        // using the normal GameSnapshot while explicitly hiding any
        // player-local selection presentation.
        m_gameState.setSelectedPosition(std::nullopt);
    }

    return result;
}


MoveResult ClientApplicationController::leaveRoom()
{
    if(!isInRoom())
    {
        return {false, "not_in_room"};
    }

    const MoveResult result = m_rooms.leaveRoom();
    if(result.success)
    {
        m_gameState.clear();
    }
    return result;
}


bool ClientApplicationController::isConnected() const
{
    return m_session.isConnected();
}


bool ClientApplicationController::isAuthenticated() const
{
    return m_session.isAuthenticated();
}


bool ClientApplicationController::isInRoom() const
{
    return m_session.hasRoom();
}
