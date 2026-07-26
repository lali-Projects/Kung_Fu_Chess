#pragma once

#include <cstddef>
#include <functional>
#include <map>
#include <memory>
#include <mutex>
#include <string>
#include <string>
#include "NetworkMessage.hpp"


class ClientConnection;
class CommandHandler;
class PlayerSession;



class ConnectionManager
{

public:


    using SendCallback =
        std::function<void(
            int,
            const NetworkMessage&)>;



public:


    explicit ConnectionManager(
        CommandHandler& commandHandler);



    ~ConnectionManager();



    ConnectionManager(
        const ConnectionManager&) = delete;



    ConnectionManager& operator=(
        const ConnectionManager&) = delete;




public:


//=================================
// Connection Management
//=================================


    int addConnection();



    bool addConnection(
        int id);



    void removeConnection(
        int id);



    void disconnectAll();




public:


//=================================
// Player
//=================================


    bool attachPlayer(
        int connectionId,
        std::shared_ptr<PlayerSession> player);




public:


//=================================
// Lookup
//=================================


    ClientConnection*
    getConnection(
        int id);



    const ClientConnection*
    getConnection(
        int id) const;



    bool containsConnection(
        int id) const;



    ClientConnection*
    findByPlayer(
        const PlayerSession& player);



    const ClientConnection*
    findByPlayer(
        const PlayerSession& player) const;




public:


//=================================
// Messaging
//=================================


    void broadcast(
        const NetworkMessage& message);



    void broadcastToRoom(
        const std::string& roomId,
        const NetworkMessage& message);




public:


//=================================
// Information
//=================================


    size_t size() const;




public:


//=================================
// Callback
//=================================


    void setSendCallback(
        SendCallback callback);




private:


    ClientConnection*
    findUnsafe(
        int id);



    const ClientConnection*
    findUnsafe(
        int id) const;




private:


    int m_nextId{1};



    CommandHandler& m_commandHandler;



    SendCallback m_sendCallback;



    mutable std::mutex
        m_connectionsMutex;



    std::map<
        int,
        std::unique_ptr<ClientConnection>>
        m_connections;

};