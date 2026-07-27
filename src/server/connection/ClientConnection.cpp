#include "ClientConnection.hpp"


#include "CommandHandler.hpp"
#include "CommandParser.hpp"
#include "Command.hpp"

#include "PlayerSession.hpp"


#include <utility>
#include <stdexcept>




//================================================
// Constructor
//================================================

ClientConnection::ClientConnection(
    int id,
    CommandHandler& commandHandler)
:
m_id(id),
m_commandHandler(commandHandler)
{

    m_commandParser =
        std::make_unique<CommandParser>();

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

    if(!m_commandParser)
    {
        return
        {
            false,
            "parser_missing"
        };
    }



    auto command =
        m_commandParser->parse(
            message.getPayload());



    if(!command)
    {
        return
        {
            false,
            "invalid_command"
        };
    }




    return
        m_commandHandler.handle(
            *this,
            *command);

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
// Callback
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

    m_lastMessage =
        message;



    if(m_sendCallback)
    {
        m_sendCallback(
            message);
    }

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

bool ClientConnection::hasPlayer() const
{

    return
        m_player != nullptr;

}







void ClientConnection::attachPlayer(
    std::shared_ptr<PlayerSession> player)
{

    if(!player)
        return;



    m_player =
        std::move(player);



    m_player->setConnectionId(
        m_id);

}








void ClientConnection::clearPlayer()
{

    if(m_player)
    {
        m_player->disconnect();
    }



    m_player.reset();

}







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







PlayerSession&
ClientConnection::getPlayerSession()
{

    if(!m_player)
    {
        throw std::runtime_error(
            "No player attached");
    }



    return *m_player;

}







//================================================
// Last Message
//================================================

const std::optional<NetworkMessage>&
ClientConnection::getLastMessage() const
{

    return m_lastMessage;

}