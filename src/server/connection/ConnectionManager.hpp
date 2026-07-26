#pragma once

#include <cstddef>
#include <functional>
#include <map>
#include <memory>
#include <mutex>


#include "NetworkMessage.hpp"



class ClientConnection;
class CommandHandler;
class GameSession;



class ConnectionManager
{

public:


    using SendCallback =
        std::function<void(
            int connectionId,
            const NetworkMessage& message)>;



public:


    explicit ConnectionManager(
        CommandHandler& commandHandler,
        GameSession& session);



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



    ClientConnection* getConnection(
        int id);



    void broadcast(
        const NetworkMessage& message);



    size_t size() const;



    void setSendCallback(
        SendCallback callback);



private:


    int m_nextId{1};


    GameSession& m_session;


    CommandHandler& m_commandHandler;



    SendCallback m_sendCallback;



    mutable std::mutex
        m_connectionsMutex;



    std::map<
        int,
        std::unique_ptr<ClientConnection>>
        m_connections;

};