#include "Server.hpp"

#include <iostream>
#include <stdexcept>

#include "INetworkServer.hpp"
#include "ConnectionManager.hpp"
#include "ClientConnection.hpp"
#include "PlayerLifecycleService.hpp"
#include "EventBus.hpp"
#include "GameStateChangedEvent.hpp"
#include "SnapshotSerializer.hpp"
#include "NetworkMessage.hpp"
#include "CommandResponseCodec.hpp"

Server::Server(
    CommandHandler& commandHandler,
    EventBus& eventBus,
    PlayerLifecycleService& playerLifecycle,
    std::unique_ptr<INetworkServer> networkServer)
    : m_commandHandler(commandHandler),
      m_eventBus(eventBus),
      m_networkServer(std::move(networkServer))
{
    m_connectionManager = std::make_unique<ConnectionManager>(m_commandHandler);

    m_connectionManager->setSendCallback(
        [this](int connectionId, const NetworkMessage& message)
        {
            if (!m_networkServer) return;
            m_networkServer->send(connectionId, message);
        });

    m_connectionManager->setPlayerDisconnectCallback(
        [&playerLifecycle](const std::shared_ptr<PlayerSession>& player)
        {
            playerLifecycle.disconnect(player);
        });

    if (m_networkServer)
    {
        m_networkServer->setConnectionCallback(
            [this](int id)
            {
                handleConnection(id);
            });

        m_networkServer->setMessageCallback(
            [this](int id, const NetworkMessage& message)
            {
                return handleNetworkMessage(id, message);
            });

        m_networkServer->setDisconnectCallback(
            [this](int id)
            {
                handleDisconnect(id);
            });
    }

    m_eventBus.subscribe<GameStateChangedEvent>(
        [this](std::shared_ptr<GameStateChangedEvent> event)
        {
            onGameStateChanged(event);
        });
}

Server::~Server()
{
    stop();

    if (m_connectionManager)
    {
        m_connectionManager->disconnectAll();
    }

    if (m_networkServer)
    {
        m_networkServer->setMessageCallback(nullptr);
        m_networkServer->setConnectionCallback(nullptr);
        m_networkServer->setDisconnectCallback(nullptr);
    }
}

void Server::start()
{
    if (m_running) return;

    if (m_networkServer)
    {
        m_networkServer->start();

        if (!m_networkServer->isRunning())
        {
            throw std::runtime_error("Network server failed to start");
        }
    }

    m_running = true;
    std::cout << "[SERVER] Started\n";
}

void Server::stop()
{
    if (!m_running) return;

    if (m_networkServer)
    {
        m_networkServer->stop();
    }

    if (m_connectionManager)
    {
        m_connectionManager->disconnectAll();
    }

    m_running = false;
    std::cout << "[SERVER] Stopped\n";
}

bool Server::isRunning() const
{
    return m_running;
}

ConnectionManager& Server::getConnectionManager()
{
    return *m_connectionManager;
}

INetworkServer& Server::getNetworkServer()
{
    if (!m_networkServer)
    {
        throw std::runtime_error("Network server unavailable");
    }

    return *m_networkServer;
}

void Server::handleConnection(int connectionId)
{
    if (!m_connectionManager) return;

    if (m_connectionManager->addConnection(connectionId))
    {
        std::cout << "[SERVER] Client connected " << connectionId << "\n";
    }
}

void Server::handleDisconnect(int connectionId)
{
    if (!m_connectionManager) return;

    m_connectionManager->removeConnection(connectionId);
}

NetworkMessage Server::handleNetworkMessage(int connectionId, const NetworkMessage& message)
{
    if (!m_connectionManager)
    {
        CommandResponse response;
        response.reason = "connection_manager_missing";
        return NetworkMessage(
            MessageType::COMMAND_RESULT,
            CommandResponseCodec::serialize(response));
    }

    auto client = m_connectionManager->getConnection(connectionId);

    if (!client)
    {
        CommandResponse response;
        response.reason = "client_missing";
        return NetworkMessage(
            MessageType::COMMAND_RESULT,
            CommandResponseCodec::serialize(response));
    }

    CommandResponse response = client->receiveNetworkMessage(message);

    return NetworkMessage(
        MessageType::COMMAND_RESULT,
        CommandResponseCodec::serialize(response));
}

void Server::onGameStateChanged(std::shared_ptr<GameStateChangedEvent> event)
{
    if (!event)
        return;

    NetworkMessage message =
        SnapshotSerializer::serialize(
            event->getSnapshot());

    m_connectionManager->broadcastToRoom(
        event->getRoomId(),
        message);
}

int Server::createTestConnection()
{
    if (!m_connectionManager) return -1;

    return m_connectionManager->addConnection();
}

void Server::closeTestConnection(int connectionId)
{
    if (!m_connectionManager) return;

    m_connectionManager->removeConnection(connectionId);
}

MoveResult Server::simulateClientCommand(int connectionId, const std::string& message)
{
    if (!m_connectionManager)
    {
        return {false, "connection_manager_missing"};
    }

    auto client = m_connectionManager->getConnection(connectionId);

    if (!client)
    {
        return {false, "client_missing"};
    }

    CommandResponse response = client->receiveNetworkMessage(
        NetworkMessage(MessageType::COMMAND, message));

    return {response.success, response.reason};
}

MoveResult Server::simulateClientCommand(const std::string& message)
{
    int id = createTestConnection();

    if (id < 0)
    {
        return {false, "connection_failed"};
    }

    MoveResult result = simulateClientCommand(id, message);

    closeTestConnection(id);

    return result;
}