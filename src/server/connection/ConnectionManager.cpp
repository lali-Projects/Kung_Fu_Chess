#include "ConnectionManager.hpp"


#include "ClientConnection.hpp"
#include "CommandHandler.hpp"
#include "GameSession.hpp"
#include <iostream>

#include <utility>



//================================================
// Constructor
//================================================

ConnectionManager::ConnectionManager(
    CommandHandler& commandHandler,
    GameSession& session)
:
m_session(session),
m_commandHandler(commandHandler)
{
}



//================================================
// Destructor
//================================================

ConnectionManager::~ConnectionManager() = default;



//================================================
// Add Local Connection
//================================================

int ConnectionManager::addConnection()
{

    int id =
        m_nextId++;


    if(addConnection(id))
    {
        return id;
    }


    return -1;
}



//================================================
// Add External Connection
//================================================

bool ConnectionManager::addConnection(
    int id)
{

    std::lock_guard<std::mutex> lock(
        m_connectionsMutex);



    if(m_connections.find(id) !=
       m_connections.end())
    {
        return false;
    }



    auto connection =
        std::make_unique<ClientConnection>(
            id,
            m_commandHandler);



    /*
        Connect logical client output
        to transport layer.

        ClientConnection remains
        transport independent.
    */
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



    m_session.addPlayer(
        connection->getPlayer());



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



    auto iterator =
        m_connections.find(id);



    if(iterator ==
       m_connections.end())
    {
        return;
    }



    m_connections.erase(
        iterator);
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
// Broadcast
//================================================

void ConnectionManager::broadcast(
    const NetworkMessage& message)
{
    std::cout
        << "[CONNECTION MANAGER] broadcast ENTER"
        << std::endl;



    std::vector<ClientConnection*> connections;



    {
        std::lock_guard<std::mutex> lock(
            m_connectionsMutex);



        std::cout
            << "[CONNECTION MANAGER] LOCK ACQUIRED"
            << std::endl;



        for(auto& pair : m_connections)
        {
            if(pair.second)
            {
                connections.push_back(
                    pair.second.get());
            }
        }
    }



    std::cout
        << "[CONNECTION MANAGER] LOCK RELEASED"
        << std::endl;



    for(auto* connection : connections)
    {

        if(connection)
        {
            std::cout
                << "[CONNECTION MANAGER] delivering"
                << std::endl;


            connection->deliverMessage(
                message);
        }

    }



    std::cout
        << "[CONNECTION MANAGER] broadcast END"
        << std::endl;
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
// Set Send Callback
//================================================

void ConnectionManager::setSendCallback(
    SendCallback callback)
{

    std::lock_guard<std::mutex> lock(
        m_connectionsMutex);



    m_sendCallback =
        std::move(callback);

}