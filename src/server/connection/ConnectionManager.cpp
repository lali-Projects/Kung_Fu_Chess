#include "ConnectionManager.hpp"


#include "ClientConnection.hpp"
#include "CommandHandler.hpp"



//================================================
// Constructor
//================================================

ConnectionManager::ConnectionManager(
    CommandHandler& commandHandler,
    GameSession& session)
:
m_commandHandler(commandHandler),
m_session(session)
{
}


//================================================
// Destructor
//================================================

ConnectionManager::~ConnectionManager()
{
}



//================================================
// Add Connection
//================================================

int ConnectionManager::addConnection()
{
    int id = m_nextId++;


    auto connection =
        std::make_unique<ClientConnection>(
            id,
            m_commandHandler);



    m_session.addPlayer(
        connection->getPlayer());



    m_connections[id] =
        std::move(connection);



    return id;
}



//================================================
// Remove Connection
//================================================

void ConnectionManager::removeConnection(
    int id)
{

    auto iterator =
        m_connections.find(id);



    if(iterator == m_connections.end())
    {
        return;
    }



    m_connections.erase(iterator);

}



//================================================
// Get Connection
//================================================

ClientConnection*
ConnectionManager::getConnection(
    int id)
{

    auto iterator =
        m_connections.find(id);



    if(iterator == m_connections.end())
    {
        return nullptr;
    }



    return iterator->second.get();

}



//================================================
// Broadcast Snapshot
//================================================





//================================================
// Size
//================================================

size_t ConnectionManager::size() const
{
    return m_connections.size();
}

void ConnectionManager::broadcastSnapshot(
    const GameSnapshot& snapshot)
{
    for(auto& [id, connection] : m_connections)
    {
        if(connection)
        {
            connection->deliverSnapshot(snapshot);
        }
    }
}