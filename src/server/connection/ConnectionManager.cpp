#include "ConnectionManager.hpp"

#include "ClientConnection.hpp"
#include "CommandHandler.hpp"
#include "PlayerSession.hpp"

#include <utility>
#include <vector>

//================================================
// Constructor
//================================================

ConnectionManager::ConnectionManager(
    CommandHandler& commandHandler)
:
m_commandHandler(commandHandler)
{
}

//================================================
// Destructor
//================================================

ConnectionManager::~ConnectionManager() = default;


//================================================
// Add External Connection
//================================================
int ConnectionManager::addConnection()
{
    int id;

    {
        std::lock_guard<std::mutex> lock(
            m_connectionsMutex);

        id = m_nextId++;
    }


    if(!addConnection(id))
    {
        return -1;
    }


    return id;
}

bool ConnectionManager::addConnection(
    int id)
{
    std::lock_guard<std::mutex> lock(
        m_connectionsMutex);

    if(findUnsafe(id))
    {
        return false;
    }

    auto connection =
        std::make_unique<ClientConnection>(
            id,
            m_commandHandler);

    connection->setSendCallback(
        [this, id]
        (
            const NetworkMessage& message
        )
        {
            SendCallback callback;

            {
                std::lock_guard<std::mutex> lock(
                    m_connectionsMutex);

                callback = m_sendCallback;
            }

            if(callback)
            {
                callback(
                    id,
                    message);
            }
        });

    m_connections.emplace(
        id,
        std::move(connection));

    return true;
}

 //================================================
// Remove Connection
//================================================

void ConnectionManager::removeConnection(
    int id)
{
    std::unique_ptr<ClientConnection> connection;



    {
        std::lock_guard<std::mutex> lock(
            m_connectionsMutex);



        auto iterator =
            m_connections.find(id);



        if(iterator == m_connections.end())
            return;



        connection =
            std::move(iterator->second);



        m_connections.erase(iterator);
    }



    if(connection)
    {
        auto player =
            connection->getPlayer();



        if(player)
        {
            player->disconnect();
        }
    }
}


//================================================
// Disconnect All
//================================================

void ConnectionManager::disconnectAll()
{
    std::vector<
        std::unique_ptr<ClientConnection>>
        connections;



    {
        std::lock_guard<std::mutex> lock(
            m_connectionsMutex);


        for(auto& [id, connection] :
            m_connections)
        {
            if(connection)
            {
                connections.push_back(
                    std::move(connection));
            }
        }


        m_connections.clear();
    }




    for(auto& connection :
        connections)
    {
        if(!connection)
            continue;


        auto player =
            connection->getPlayer();



        if(player)
        {
            player->disconnect();
        }
    }
}



//================================================
// Get Connection
//================================================

ClientConnection*
ConnectionManager::getConnection(
    int id)
{
    std::lock_guard<std::mutex> lock(
        m_connectionsMutex);


    return findUnsafe(id);
}




const ClientConnection* ConnectionManager::getConnection(
    int id) const
{
    std::lock_guard<std::mutex> lock(
        m_connectionsMutex);


    return findUnsafe(id);
}




//================================================
// Contains Connection
//================================================

bool ConnectionManager::containsConnection(
    int id) const
{
    std::lock_guard<std::mutex> lock(
        m_connectionsMutex);


    return
        findUnsafe(id)
        !=
        nullptr;
}




//================================================
// Find Unsafe
//================================================

ClientConnection*
ConnectionManager::findUnsafe(
    int id)
{
    auto iterator =
        m_connections.find(id);


    if(iterator ==
       m_connections.end())
    {
        return nullptr;
    }


    return iterator->second.get();
}




const ClientConnection*
ConnectionManager::findUnsafe(
    int id) const
{
    auto iterator =
        m_connections.find(id);


    if(iterator ==
       m_connections.end())
    {
        return nullptr;
    }


    return iterator->second.get();
}

 //================================================
// Find Connection By Player
//================================================

ClientConnection*
ConnectionManager::findByPlayer(
    const PlayerSession& player)
{
    std::lock_guard<std::mutex> lock(
        m_connectionsMutex);


    for(auto& [id, connection] :
        m_connections)
    {
        if(!connection)
        {
            continue;
        }


        auto session =
            connection->getPlayer();


        if(session &&
           session.get() == &player)
        {
            return connection.get();
        }
    }


    return nullptr;
}




const ClientConnection*
ConnectionManager::findByPlayer(
    const PlayerSession& player) const
{
    std::lock_guard<std::mutex> lock(
        m_connectionsMutex);


    for(const auto& [id, connection] :
        m_connections)
    {
        if(!connection)
        {
            continue;
        }


        auto session =
            connection->getPlayer();


        if(session &&
           session.get() == &player)
        {
            return connection.get();
        }
    }


    return nullptr;
}




//================================================
// Broadcast All Clients
//================================================

void ConnectionManager::broadcast(
    const NetworkMessage& message)
{
    std::vector<ClientConnection*> clients;


    {
        std::lock_guard<std::mutex> lock(
            m_connectionsMutex);


        for(auto& [id, connection] :
            m_connections)
        {
            if(connection)
            {
                clients.push_back(
                    connection.get());
            }
        }
    }



    /*
        Send outside mutex.

        Prevents deadlocks if
        delivery triggers callbacks.
    */

    for(auto* client : clients)
    {
        if(client)
        {
            client->deliverMessage(
                message);
        }
    }
}




//================================================
// Broadcast To Room
//================================================

void ConnectionManager::broadcastToRoom(
    const std::string& roomId,
    const NetworkMessage& message)
{
    std::vector<ClientConnection*> clients;


    {
        std::lock_guard<std::mutex> lock(
            m_connectionsMutex);


        for(auto& [id, connection] :
            m_connections)
        {
            if(!connection)
            {
                continue;
            }


            auto player =
                connection->getPlayer();


            if(!player)
            {
                continue;
            }


            if(player->getRoomId()
                ==
               roomId)
            {
                clients.push_back(
                    connection.get());
            }
        }
    }

    for(auto* client : clients)
    {
        if(client)
        {
            client->deliverMessage(
                message);
        }
    }
}
 //================================================
// Size
//================================================

size_t ConnectionManager::size() const
{
    std::lock_guard<std::mutex> lock(
        m_connectionsMutex);


    return m_connections.size();
}




//================================================
// Send Callback
//================================================

void ConnectionManager::setSendCallback(
    SendCallback callback)
{
    std::lock_guard<std::mutex> lock(
        m_connectionsMutex);


    m_sendCallback =
        std::move(callback);
}

bool ConnectionManager::attachPlayer(
    int connectionId,
    std::shared_ptr<PlayerSession> player)
{

    if(!player)
        return false;



    ClientConnection* connection =
        getConnection(connectionId);



    if(!connection)
        return false;



    if(connection->getPlayer())
        return false;



    connection->attachPlayer(
        std::move(player));



    return true;
}