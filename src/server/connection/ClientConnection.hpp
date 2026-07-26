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
 * @brief Represents one logical client connection.
 *
 * Responsibilities:
 *
 *  - Own PlayerSession.
 *  - Receive commands.
 *  - Forward commands to CommandHandler.
 *  - Send messages.
 *  - Store last outgoing message for tests.
 *
 *
 * Does NOT know:
 *
 *  - GameEngine.
 *  - Board.
 *  - Rules.
 *  - Rooms.
 *  - Server.
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



    std::optional<NetworkMessage>
        m_lastMessage;



    SendCallback m_sendCallback;

};