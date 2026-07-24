#pragma once


#include <memory>
#include <optional>


#include "NetworkMessage.hpp"
#include "MoveResult.hpp"



class ConnectionHandler;
class CommandHandler;
class PlayerSession;




/**
 * @brief Represents one connected client.
 *
 * Responsibilities:
 *
 *  - Own player identity.
 *  - Receive network messages.
 *  - Forward commands.
 *  - Deliver outgoing messages.
 *
 *
 * Does NOT know:
 *
 *  - GameEngine.
 *  - Board.
 *  - Rules.
 *  - Snapshots.
 *  - Serialization.
 *  - Network implementation.
 */
class ClientConnection
{

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


    MoveResult send(
        const NetworkMessage& message);



    MoveResult receiveNetworkMessage(
        const NetworkMessage& message);




    /**
     * @brief Delivers outgoing message.
     *
     * Current:
     * Stores locally.
     *
     * Future:
     * WebSocket transport.
     */
    void deliverMessage(
        const NetworkMessage& message);




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
        Testing storage only.

        Future:
        replaced by WebSocket transport.
    */
    std::optional<NetworkMessage>
        m_lastMessage;

};