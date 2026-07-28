#include "ConnectionManager.hpp"

#include "ClientConnection.hpp"
#include "CommandHandler.hpp"
#include "PlayerSession.hpp"

#include <utility>

//================================================
// Constructor
//================================================

ConnectionManager::ConnectionManager(CommandHandler& commandHandler)
    : m_commandHandler(commandHandler)
{
}

//================================================
// Destructor
//================================================

ConnectionManager::~ConnectionManager()
{
    disconnectAll();
}

//================================================
// Add Connection
//================================================

int ConnectionManager::addConnection()
{
    int id = m_nextId.fetch_add(1);

    if (!addConnection(id))
    {
        return -1;
    }

    return id;
}

bool ConnectionManager::addConnection(int connectionId)
{
    auto connection = std::make_shared<ClientConnection>(connectionId, m_commandHandler);

    connection->setSendCallback([this, connectionId](const NetworkMessage& message) {
        SendCallback callback;

        {
            std::shared_lock lock(m_connectionsMutex);
            callback = m_sendCallback;
        }

        if (callback)
        {
            callback(connectionId, message);
        }
    });

    {
        std::unique_lock lock(m_connectionsMutex);

        if (m_connections.find(connectionId) != m_connections.end())
        {
            return false;
        }

        m_connections.emplace(connectionId, std::move(connection));
    }

    return true;
}

//================================================
// Remove
//================================================

void ConnectionManager::removeConnection(int connectionId)
{
    std::shared_ptr<ClientConnection> connection;

    {
        std::unique_lock lock(m_connectionsMutex);

        auto it = m_connections.find(connectionId);
        if (it == m_connections.end())
        {
            return;
        }

        connection = it->second;
        m_connections.erase(it);
    }

    if (connection)
    {
        auto player = connection->getPlayer();

        if (player)
        {
            PlayerDisconnectCallback callback;

            {
                std::shared_lock lock(m_connectionsMutex);
                callback = m_playerDisconnectCallback;
            }

            if (callback)
            {
                callback(player);
            }
            else
            {
                player->disconnect();
            }
        }
    }
}

//================================================
// Disconnect All
//================================================

void ConnectionManager::disconnectAll()
{
    std::vector<std::shared_ptr<ClientConnection>> connections;
    PlayerDisconnectCallback callback;

    {
        std::unique_lock lock(m_connectionsMutex);

        for (auto& [id, connection] : m_connections)
        {
            connections.push_back(connection);
        }

        m_connections.clear();
        callback = m_playerDisconnectCallback;
    }

    for (auto& connection : connections)
    {
        if (!connection)
            continue;

        auto player = connection->getPlayer();

        if (player)
        {
            if (callback)
            {
                callback(player);
            }
            else
            {
                player->disconnect();
            }
        }
    }
}

//================================================
// Attach Player
//================================================

bool ConnectionManager::attachPlayer(int connectionId, std::shared_ptr<PlayerSession> player)
{
    if (!player)
        return false;

    auto connection = getConnection(connectionId);

    if (!connection)
        return false;

    if (connection->hasPlayer())
        return false;

    if (!connection->attachPlayer(std::move(player)))
    {
        return false;
    }

    return true;
}

//================================================
// Get Connection
//================================================

std::shared_ptr<ClientConnection> ConnectionManager::getConnection(int connectionId)
{
    std::shared_lock lock(m_connectionsMutex);
    return findUnsafe(connectionId);
}

std::shared_ptr<const ClientConnection> ConnectionManager::getConnection(int connectionId) const
{
    std::shared_lock lock(m_connectionsMutex);
    return findUnsafe(connectionId);
}

//================================================
// Find Unsafe
//================================================

std::shared_ptr<ClientConnection> ConnectionManager::findUnsafe(int connectionId)
{
    auto it = m_connections.find(connectionId);

    if (it == m_connections.end())
    {
        return nullptr;
    }

    return it->second;
}

std::shared_ptr<const ClientConnection> ConnectionManager::findUnsafe(int connectionId) const
{
    auto it = m_connections.find(connectionId);

    if (it == m_connections.end())
    {
        return nullptr;
    }

    return it->second;
}

//================================================
// Contains
//================================================

bool ConnectionManager::containsConnection(int connectionId) const
{
    std::shared_lock lock(m_connectionsMutex);
    return m_connections.find(connectionId) != m_connections.end();
}

//================================================
// Find Player
//================================================

std::shared_ptr<ClientConnection> ConnectionManager::findByPlayer(const PlayerSession& player)
{
    std::shared_lock lock(m_connectionsMutex);

    for (auto& [id, connection] : m_connections)
    {
        if (!connection)
            continue;

        auto session = connection->getPlayer();

        if (session && session.get() == &player)
        {
            return connection;
        }
    }

    return nullptr;
}

//================================================
// Broadcast
//================================================

void ConnectionManager::broadcast(const NetworkMessage& message)
{
    std::vector<std::shared_ptr<ClientConnection>> connections;

    {
        std::shared_lock lock(m_connectionsMutex);

        for (auto& [id, connection] : m_connections)
        {
            connections.push_back(connection);
        }
    }

    for (auto& connection : connections)
    {
        if (connection)
        {
            connection->deliverMessage(message);
        }
    }
}

//================================================
// Broadcast Room
//================================================

void ConnectionManager::broadcastToRoom(const std::string& roomId, const NetworkMessage& message)
{
    std::vector<std::shared_ptr<ClientConnection>> connections;

    {
        std::shared_lock lock(m_connectionsMutex);

        for (auto& [id, connection] : m_connections)
        {
            if (!connection)
                continue;

            auto player = connection->getPlayer();

            if (player && player->getRoomId() == roomId)
            {
                connections.push_back(connection);
            }
        }
    }

    for (auto& connection : connections)
    {
        connection->deliverMessage(message);
    }
}

//================================================
// Size
//================================================

size_t ConnectionManager::size() const
{
    std::shared_lock lock(m_connectionsMutex);
    return m_connections.size();
}

//================================================
// Callback
//================================================

void ConnectionManager::setSendCallback(SendCallback callback)
{
    std::unique_lock lock(m_connectionsMutex);
    m_sendCallback = std::move(callback);
}

void ConnectionManager::setPlayerDisconnectCallback(PlayerDisconnectCallback callback)
{
    std::unique_lock lock(m_connectionsMutex);
    m_playerDisconnectCallback = std::move(callback);
}