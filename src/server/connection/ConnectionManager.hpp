#pragma once


#include <cstddef>
#include <functional>
#include <map>
#include <memory>
#include <mutex>
#include <string>


#include "NetworkMessage.hpp"



class ClientConnection;
class CommandHandler;



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


    int addConnection();



    bool addConnection(
        int id);



    void removeConnection(
        int id);



    ClientConnection*
    getConnection(
        int id);



    void broadcast(
        const NetworkMessage& message);



    void broadcastToRoom(
        const std::string& roomId,
        const NetworkMessage& message);



    size_t size() const;



    void setSendCallback(
        SendCallback callback);



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