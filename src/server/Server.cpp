#include "Server.hpp"


#include <iostream>
#include <stdexcept>


#include "INetworkServer.hpp"
#include "ConnectionManager.hpp"
#include "ClientConnection.hpp"


#include "EventBus.hpp"
#include "GameStateChangedEvent.hpp"


#include "SnapshotSerializer.hpp"
#include "NetworkMessage.hpp"




//================================================
// Constructor
//================================================

Server::Server(
    CommandHandler& commandHandler,
    EventBus& eventBus,
    std::unique_ptr<INetworkServer> networkServer)
:
m_commandHandler(commandHandler),
m_eventBus(eventBus),
m_networkServer(std::move(networkServer))
{


    m_connectionManager =
        std::make_unique<ConnectionManager>(
            m_commandHandler);



    //---------------------------------
    // Outgoing flow
    //
    // GameSession
    //      |
    // EventBus
    //      |
    // Server
    //      |
    // ConnectionManager
    //      |
    // Network
    //---------------------------------

    m_connectionManager->setSendCallback(
        [this]
        (
            int connectionId,
            const NetworkMessage& message
        )
        {

            if(!m_networkServer)
                return;


            m_networkServer->send(
                connectionId,
                message);

        });



    //---------------------------------
    // Network callbacks
    //---------------------------------

    if(m_networkServer)
    {

        m_networkServer->setConnectionCallback(
            [this](int id)
            {
                handleConnection(id);
            });



        m_networkServer->setMessageCallback(
            [this]
            (
                int id,
                const NetworkMessage& message
            )
            {
                handleNetworkMessage(
                    id,
                    message);
            });



        m_networkServer->setDisconnectCallback(
            [this](int id)
            {
                handleDisconnect(id);
            });

    }



    //---------------------------------
    // Game events
    //---------------------------------

    m_eventBus.subscribe<GameStateChangedEvent>(
        [this]
        (
            std::shared_ptr<GameStateChangedEvent> event
        )
        {
            onGameStateChanged(event);
        });

}






Server::~Server()
{
    stop();

    if(m_networkServer)
    {
        m_networkServer->setMessageCallback(nullptr);
        m_networkServer->setConnectionCallback(nullptr);
        m_networkServer->setDisconnectCallback(nullptr);
    }
}





void Server::start()
{

    if(m_running)
        return;



    if(m_networkServer)
        m_networkServer->start();



    m_running = true;



    std::cout
        << "[SERVER] Started\n";

}






void Server::stop()
{

    if(m_networkServer)
    {
        m_networkServer->stop();
    }


    m_running = false;


    std::cout
        << "[SERVER] Stopped\n";
}






bool Server::isRunning() const
{
    return m_running;
}






ConnectionManager&
Server::getConnectionManager()
{
    return *m_connectionManager;
}






INetworkServer&
Server::getNetworkServer()
{

    if(!m_networkServer)
    {
        throw std::runtime_error(
            "Network server unavailable");
    }


    return *m_networkServer;
}







void Server::handleConnection(
    int connectionId)
{

    if(!m_connectionManager)
        return;



    if(
        m_connectionManager->addConnection(
            connectionId))
    {

        std::cout
            << "[SERVER] Client connected "
            << connectionId
            << "\n";

    }

}






void Server::handleDisconnect(
    int connectionId)
{

    if(!m_connectionManager)
        return;



    m_connectionManager->removeConnection(
        connectionId);

}







void Server::handleNetworkMessage(
    int connectionId,
    const NetworkMessage& message)
{

    ClientConnection* client =
        m_connectionManager->getConnection(
            connectionId);



    if(!client)
        return;



    MoveResult result =
        client->receiveNetworkMessage(
            message);




    std::string text;



    if(result.success)
    {
        text =
            "RESULT SUCCESS "
            +
            result.reason;
    }
    else
    {
        text =
            "RESULT FAILED "
            +
            result.reason;
    }



    client->deliverMessage(
        NetworkMessage(
            MessageType::COMMAND_RESULT,
            text));

}







MoveResult Server::simulateClientCommand(
    const std::string& message)
{

    int id =
        createTestConnection();



    if(id < 0)
    {
        return
        {
            false,
            "connection_failed"
        };
    }



    MoveResult result =
        simulateClientCommand(
            id,
            message);



    closeTestConnection(id);



    return result;
}







void Server::onGameStateChanged(
    std::shared_ptr<Event> event)
{

    if(!event)
        return;



    auto snapshotEvent =
        std::dynamic_pointer_cast<GameStateChangedEvent>(
            event);



    if(!snapshotEvent)
        return;



    NetworkMessage message =
        SnapshotSerializer::serialize(
            snapshotEvent->getSnapshot());



    m_connectionManager->broadcastToRoom(
        snapshotEvent->getRoomId(),
        message);

}

//================================================
// Create Test Connection
//================================================

int Server::createTestConnection()
{
    if(!m_connectionManager)
    {
        return -1;
    }


    return
        m_connectionManager->addConnection();
}






//================================================
// Close Test Connection
//================================================

void Server::closeTestConnection(
    int connectionId)
{
    if(!m_connectionManager)
        return;


    m_connectionManager->removeConnection(
        connectionId);
}






//================================================
// Simulate Command With Connection
//================================================

MoveResult Server::simulateClientCommand(
    int connectionId,
    const std::string& message)
{

    if(!m_connectionManager)
    {
        return
        {
            false,
            "connection_manager_missing"
        };
    }



    ClientConnection* client =
        m_connectionManager->getConnection(
            connectionId);



    if(!client)
    {
        return
        {
            false,
            "client_missing"
        };
    }



    return
        client->receiveNetworkMessage(
            NetworkMessage(
                MessageType::COMMAND,
                message));
}