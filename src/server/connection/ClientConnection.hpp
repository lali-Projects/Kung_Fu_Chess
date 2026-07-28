#pragma once

#include <memory>
#include <mutex>
#include <optional>
#include <functional>

#include "NetworkMessage.hpp"
#include "CommandResponse.hpp"
#include "MoveResult.hpp"

class CommandHandler;
class CommandParser;
class PlayerSession;

/**
 * @brief Represents a single client connection on the server.
 *
 * Handles incoming network messages, parses commands, associates the connection
 * with a player session, and sends outgoing messages through a registered callback.
 */

class ClientConnection
{
public:
    using SendCallback = std::function<void(const NetworkMessage&)>;

    ClientConnection(int id, CommandHandler& commandHandler);
    ~ClientConnection();

    ClientConnection(const ClientConnection&) = delete;
    ClientConnection& operator=(const ClientConnection&) = delete;

    //=================================
    // Messages
    //=================================
    CommandResponse receiveNetworkMessage(const NetworkMessage& message);
    void deliverMessage(const NetworkMessage& message);
    void setSendCallback(SendCallback callback);

    //=================================
    // Player
    //=================================
    bool attachPlayer(std::shared_ptr<PlayerSession> player);
    void clearPlayer();
    bool hasPlayer() const;
    std::shared_ptr<PlayerSession> getPlayer();
    std::shared_ptr<const PlayerSession> getPlayer() const;
    PlayerSession& getPlayerSession();

    //=================================
    // Information
    //=================================
    int getId() const;
    std::optional<NetworkMessage> getLastMessage() const;

private:
    void sendMessageToClient(const NetworkMessage& message);

    mutable std::mutex m_mutex;
    int m_id;
    CommandHandler& m_commandHandler;
    std::unique_ptr<CommandParser> m_commandParser;
    std::shared_ptr<PlayerSession> m_player;
    std::optional<NetworkMessage> m_lastMessage;
    SendCallback m_sendCallback;
};