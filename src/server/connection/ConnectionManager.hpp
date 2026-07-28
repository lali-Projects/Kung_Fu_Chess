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

/** 
 *  @brief Manages active client connections and their associated player sessions. *
 * 
 *  Provides thread-safe connection lookup, player attachment, message broadcasting,
 *   and callbacks for sending network messages and handling player disconnections.
  */
class ConnectionManager
{
public:
    using SendCallback = std::function<void(int connectionId, const NetworkMessage& message)>;
    using PlayerDisconnectCallback = std::function<void(const std::shared_ptr<PlayerSession>& player)>;

    explicit ConnectionManager(CommandHandler& commandHandler);
    ~ConnectionManager();

    ConnectionManager(const ConnectionManager&) = delete;
    ConnectionManager& operator=(const ConnectionManager&) = delete;

    //=================================
    // Connection Management
    //=================================
    int addConnection();
    bool addConnection(int connectionId);
    void removeConnection(int connectionId);
    void disconnectAll();

    //=================================
    // Player
    //=================================
    bool attachPlayer(int connectionId, std::shared_ptr<PlayerSession> player);

    //=================================
    // Lookup
    //=================================
    std::shared_ptr<ClientConnection> getConnection(int connectionId);
    std::shared_ptr<const ClientConnection> getConnection(int connectionId) const;
    std::shared_ptr<ClientConnection> findByPlayer(const PlayerSession& player);
    bool containsConnection(int connectionId) const;

    //=================================
    // Messaging
    //=================================
    void broadcast(const NetworkMessage& message);
    void broadcastToRoom(const std::string& roomId, const NetworkMessage& message);

    //=================================
    // Information
    //=================================
    size_t size() const;

    //=================================
    // Network
    //=================================
    void setSendCallback(SendCallback callback);
    void setPlayerDisconnectCallback(PlayerDisconnectCallback callback);

    std::shared_ptr<ClientConnection> findUnsafe(int connectionId);
    std::shared_ptr<const ClientConnection> findUnsafe(int connectionId) const;

    std::atomic<int> m_nextId{1};
    CommandHandler& m_commandHandler;
    SendCallback m_sendCallback;
    PlayerDisconnectCallback m_playerDisconnectCallback;
    mutable std::shared_mutex m_connectionsMutex;
    std::unordered_map<int, std::shared_ptr<ClientConnection>> m_connections;
};