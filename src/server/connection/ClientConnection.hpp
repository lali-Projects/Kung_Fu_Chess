#pragma once


#include <memory>
#include <optional>
#include <functional>


#include "NetworkMessage.hpp"
#include "MoveResult.hpp"



class ConnectionHandler;
class CommandHandler;
class PlayerSession;



/**
 * @brief Represents one connected client.
 *
 * Logical client connection.
 *
 * Responsibilities:
 *
 *  - Own player identity.
 *  - Receive incoming messages.
 *  - Forward commands to CommandHandler.
 *  - Deliver outgoing messages through transport callback.
 *  - Keep local message storage for tests.
 *
 *
 * Does NOT know:
 *
 *  - GameEngine.
 *  - Board.
 *  - Rules.
 *  - Snapshots.
 *  - Serialization.
 *  - WebSocket.
 */
class ClientConnection
{

public:


    using SendCallback =
        std::function<void(
            const NetworkMessage&)>;



public:


    ClientConnection(
        int id,
        CommandHandler& commandHandler);



    ~ClientConnection();



    ClientConnection(
        const ClientConnection&) = delete;



    ClientConnection& operator=(
        const ClientConnection&) = delete;



public:


    MoveResult receiveNetworkMessage(
        const NetworkMessage& message);



    void deliverMessage(
        const NetworkMessage& message);



    void setSendCallback(
        SendCallback callback);



    const std::optional<NetworkMessage>&
    getLastMessage() const;



    int getId() const;



    std::shared_ptr<PlayerSession>
    getPlayer();



    std::shared_ptr<const PlayerSession>
    getPlayer() const;



    PlayerSession&
    getPlayerSession();



private:


    void sendMessageToClient(
        const NetworkMessage& message);



private:


    int m_id;



    std::shared_ptr<PlayerSession>
        m_player;



    std::unique_ptr<ConnectionHandler>
        m_handler;



    /*
        Used for:
        - tests
        - LocalNetworkServer
    */
    std::optional<NetworkMessage>
        m_lastMessage;



    /*
        Transport callback.

        ConnectionManager connects it
        to INetworkServer::send().
    */
    SendCallback m_sendCallback;

};