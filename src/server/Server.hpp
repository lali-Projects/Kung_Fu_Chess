#pragma once

#include <atomic>
#include <memory>
#include <string>

#include "MoveResult.hpp"

// Forward declarations
class CommandHandler;
class ConnectionManager;
class EventBus;
class Event;
class INetworkServer;
class NetworkMessage;
class PlayerLifecycleService;
class GameStateChangedEvent;

/**
 * @brief Application server facade.
 *
 * Responsibilities:
 * - Manage application lifecycle.
 * - Connect network layer with application layer.
 * - Route client messages.
 * - Broadcast game events.
 */
class Server
{
public:
    /**
     * @brief Constructs a new Server instance and initializes dependencies.
     * @param commandHandler Reference to the application command handler.
     * @param eventBus Reference to the central event bus.
     * @param playerLifecycle Reference to the player lifecycle service.
     * @param networkServer Unique pointer to the network server implementation.
     */
    Server(
        CommandHandler& commandHandler,
        EventBus& eventBus,
        PlayerLifecycleService& playerLifecycle,
        std::unique_ptr<INetworkServer> networkServer);

    ~Server();

    Server(const Server&) = delete;
    Server& operator=(const Server&) = delete;

public:
    
    void start();

    void stop();

    bool isRunning() const;

    ConnectionManager& getConnectionManager();
    INetworkServer& getNetworkServer();

    int createTestConnection();

    void closeTestConnection(int connectionId);

    MoveResult simulateClientCommand( int connectionId, const std::string& message);

    MoveResult simulateClientCommand(const std::string& message);

private:
    NetworkMessage handleNetworkMessage(
        int connectionId,
        const NetworkMessage& message);

    void handleConnection(int connectionId);

    void handleDisconnect(int connectionId);

    void onGameStateChanged(std::shared_ptr<GameStateChangedEvent> event);

    CommandHandler& m_commandHandler;
    EventBus& m_eventBus;
    std::unique_ptr<ConnectionManager> m_connectionManager;
    std::unique_ptr<INetworkServer> m_networkServer;
    std::atomic<bool> m_running{false};
};