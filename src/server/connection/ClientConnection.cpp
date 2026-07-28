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

ClientConnection::ClientConnection(int id, CommandHandler& commandHandler)
    : m_id(id), m_commandHandler(commandHandler)
{
    m_commandParser = std::make_unique<CommandParser>();
}

//================================================
// Destructor
//================================================

ClientConnection::~ClientConnection() = default;

//================================================
// Receive Network Message
//================================================

CommandResponse ClientConnection::receiveNetworkMessage(const NetworkMessage& message)
{
    if (!m_commandParser)
    {
        CommandResponse response;
        response.reason = "parser_missing";
        return response;
    }

    auto command = m_commandParser->parse(message.getPayload());

    if (!command)
    {
        CommandResponse response;
        response.reason = "invalid_command";
        return response;
    }

    return m_commandHandler.handle(*this, *command);
}

//================================================
// Deliver Message
//================================================

void ClientConnection::deliverMessage(const NetworkMessage& message)
{
    sendMessageToClient(message);
}

//================================================
// Callback
//================================================

void ClientConnection::setSendCallback(SendCallback callback)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    m_sendCallback = std::move(callback);
}

//================================================
// Send
//================================================

void ClientConnection::sendMessageToClient(const NetworkMessage& message)
{
    SendCallback sendCallback;

    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_lastMessage = message;
        sendCallback = m_sendCallback;
    }

    if (sendCallback)
    {
        sendCallback(message);
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
    std::lock_guard<std::mutex> lock(m_mutex);
    return m_player != nullptr;
}

bool ClientConnection::attachPlayer(std::shared_ptr<PlayerSession> player)
{
    if (!player)
    {
        return false;
    }

    player->setConnectionId(m_id);

    std::lock_guard<std::mutex> lock(m_mutex);

    if (m_player)
    {
        return false;
    }

    m_player = std::move(player);
    return true;
}

void ClientConnection::clearPlayer()
{
    std::shared_ptr<PlayerSession> player;

    {
        std::lock_guard<std::mutex> lock(m_mutex);
        player = std::move(m_player);
    }

    if (player)
    {
        player->disconnect();
    }
}

std::shared_ptr<PlayerSession> ClientConnection::getPlayer()
{
    std::lock_guard<std::mutex> lock(m_mutex);
    return m_player;
}

std::shared_ptr<const PlayerSession> ClientConnection::getPlayer() const
{
    std::lock_guard<std::mutex> lock(m_mutex);
    return m_player;
}

PlayerSession& ClientConnection::getPlayerSession()
{
    std::lock_guard<std::mutex> lock(m_mutex);

    if (!m_player)
    {
        throw std::runtime_error("No player attached");
    }

    return *m_player;
}

//================================================
// Last Message
//================================================

std::optional<NetworkMessage> ClientConnection::getLastMessage() const
{
    std::lock_guard<std::mutex> lock(m_mutex);
    return m_lastMessage;
}