#include "ClientConnection.hpp"


#include "ConnectionHandler.hpp"
#include "CommandHandler.hpp"
#include "PlayerSession.hpp"


#include <utility>





//================================================
// Constructor
//================================================

ClientConnection::ClientConnection(
    int id,
    CommandHandler& commandHandler)
:
m_id(id)
{


    m_player =
        std::make_shared<PlayerSession>(
            std::to_string(id));



    m_player->setConnectionId(id);



    m_player->setState(
        PlayerSession::ConnectionState::CONNECTED);



    m_handler =
        std::make_unique<ConnectionHandler>(
            commandHandler,
            *m_player);

}






//================================================
// Destructor
//================================================

ClientConnection::~ClientConnection() = default;







//================================================
// Receive Network Message
//================================================

MoveResult ClientConnection::receiveNetworkMessage(
    const NetworkMessage& message)
{

    if(!m_handler)
    {
        return
        {
            false,
            "connection_handler_missing"
        };
    }



    return m_handler->receive(
        message.getPayload());

}







//================================================
// Deliver Message
//================================================

void ClientConnection::deliverMessage(
    const NetworkMessage& message)
{

    sendMessageToClient(
        message);

}







//================================================
// Set Send Callback
//================================================

void ClientConnection::setSendCallback(
    SendCallback callback)
{

    m_sendCallback =
        std::move(callback);

}







//================================================
// Send
//================================================

void ClientConnection::sendMessageToClient(
    const NetworkMessage& message)
{


    /*
        Used by tests.
    */

    m_lastMessage =
        message;



    if(m_sendCallback)
    {
        m_sendCallback(
            message);
    }

}







//================================================
// Last Message
//================================================

const std::optional<NetworkMessage>&
ClientConnection::getLastMessage() const
{
    return m_lastMessage;
}







//================================================
// ID
//================================================

int ClientConnection::getId() const
{
    return m_id;
}







//================================================
// Player
//================================================

std::shared_ptr<PlayerSession>
ClientConnection::getPlayer()
{
    return m_player;
}






std::shared_ptr<const PlayerSession>
ClientConnection::getPlayer() const
{
    return m_player;
}







//================================================
// Player Session
//================================================

PlayerSession&
ClientConnection::getPlayerSession()
{

    if(!m_player)
    {
        throw std::runtime_error(
            "ClientConnection has no PlayerSession");
    }


    return *m_player;

}