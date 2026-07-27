#pragma once


#include <atomic>
#include <functional>
#include <memory>
#include <mutex>
#include <shared_mutex>
#include <string>
#include <unordered_map>
#include <vector>


#include "NetworkMessage.hpp"



class ClientConnection;
class CommandHandler;
class PlayerSession;



class ConnectionManager
{

public:


    using SendCallback =
        std::function<void(
            int connectionId,
            const NetworkMessage& message)>;


    using PlayerDisconnectCallback =
        std::function<void(
            const std::shared_ptr<PlayerSession>& player)>;



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
        int connectionId);



    void removeConnection(
        int connectionId);



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


    std::shared_ptr<ClientConnection>
    getConnection(
        int connectionId);



    std::shared_ptr<const ClientConnection>
    getConnection(
        int connectionId) const;



    std::shared_ptr<ClientConnection>
    findByPlayer(
        const PlayerSession& player);



    bool containsConnection(
        int connectionId) const;





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
// Network
//=================================


    void setSendCallback(
        SendCallback callback);


    void setPlayerDisconnectCallback(
        PlayerDisconnectCallback callback);





private:


    std::shared_ptr<ClientConnection>
    findUnsafe(
        int connectionId);



    std::shared_ptr<const ClientConnection>
    findUnsafe(
        int connectionId) const;





private:


    std::atomic<int>
        m_nextId{1};



    CommandHandler& 
        m_commandHandler;



    SendCallback
        m_sendCallback;


    PlayerDisconnectCallback
        m_playerDisconnectCallback;



    mutable std::shared_mutex
        m_connectionsMutex;



    std::unordered_map<
        int,
        std::shared_ptr<ClientConnection>>
        m_connections;

};
