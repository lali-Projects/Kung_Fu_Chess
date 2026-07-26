#include "ConnectionManager.hpp"


#include "ClientConnection.hpp"
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
// Add Automatic Connection
//================================================

int ConnectionManager::addConnection()
{

    std::lock_guard<std::mutex> lock(
        m_connectionsMutex);



    int id =
        m_nextId++;



    auto connection =
        std::make_unique<ClientConnection>(
            id,
            m_commandHandler);




    connection->setSendCallback(
        [this,id]
        (
            const NetworkMessage& message
        )
        {

            SendCallback callback;



            {
                std::lock_guard<std::mutex> lock(
                    m_connectionsMutex);


                callback =
                    m_sendCallback;
            }



            if(callback)
            {
                callback(
                    id,
                    message);
            }

        });




    m_connections[id] =
        std::move(connection);



    return id;

}






//================================================
// Add External Connection
//================================================

bool ConnectionManager::addConnection(
    int id)
{

    std::lock_guard<std::mutex> lock(
        m_connectionsMutex);



    if (m_connections.find(id) != m_connections.end())
{
    return false;
}



    auto connection =
        std::make_unique<ClientConnection>(
            id,
            m_commandHandler);




    connection->setSendCallback(
        [this,id]
        (
            const NetworkMessage& message
        )
        {

            SendCallback callback;



            {
                std::lock_guard<std::mutex> lock(
                    m_connectionsMutex);


                callback =
                    m_sendCallback;
            }



            if(callback)
            {
                callback(
                    id,
                    message);
            }

        });




    m_connections[id] =
        std::move(connection);



    return true;

}






//================================================
// Remove Connection
//================================================

void ConnectionManager::removeConnection(
    int id)
{

    std::lock_guard<std::mutex> lock(
        m_connectionsMutex);



    m_connections.erase(id);

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
// Broadcast All Clients
//================================================

void ConnectionManager::broadcast(
    const NetworkMessage& message)
{

    std::vector<ClientConnection*> clients;



    {
        std::lock_guard<std::mutex> lock(
            m_connectionsMutex);



        for(auto& [id,connection] :
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
        deliverMessage triggers callbacks.
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
// Broadcast Room
//================================================

void ConnectionManager::broadcastToRoom(
    const std::string& roomId,
    const NetworkMessage& message)
{

    std::vector<ClientConnection*> clients;



    {
        std::lock_guard<std::mutex> lock(
            m_connectionsMutex);



        for(auto& [id,connection] :
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