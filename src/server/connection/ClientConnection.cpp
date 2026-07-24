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
// Send
//================================================

MoveResult ClientConnection::send(
    const NetworkMessage& message)
{
    return receiveNetworkMessage(
        message);
}




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
// Store Outgoing Message
//================================================

void ClientConnection::sendMessageToClient(
    const NetworkMessage& message)
{

    m_lastMessage =
        message;

}




//================================================
// Get Last Message
//================================================

const std::optional<NetworkMessage>&
ClientConnection::getLastMessage() const
{
    return m_lastMessage;
}




//================================================
// Get Id
//================================================

int ClientConnection::getId() const
{
    return m_id;
}




//================================================
// Get Player
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
// Get Player Session
//================================================

PlayerSession&
ClientConnection::getPlayerSession()
{
    return *m_player;
}