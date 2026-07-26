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
 * @brief Represents one client connection.
 *
 * Responsibilities:
 *
 *  - Own connection state.
 *  - Receive messages.
 *  - Deliver outgoing messages.
 *  - Hold attached PlayerSession.
 *
 *
 * Does NOT know:
 *
 *  - Database.
 *  - Authentication.
 *  - Rooms.
 *  - Game rules.
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



public:


    int getId() const;



    bool hasPlayer() const;



    void attachPlayer(
        std::shared_ptr<PlayerSession> player);



    void clearPlayer();



    std::shared_ptr<PlayerSession>
    getPlayer();



    std::shared_ptr<const PlayerSession>
    getPlayer() const;



    PlayerSession&
    getPlayerSession();



public:


    const std::optional<NetworkMessage>&
    getLastMessage() const;



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