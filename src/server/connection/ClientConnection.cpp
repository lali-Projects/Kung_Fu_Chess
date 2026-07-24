#include "ClientConnection.hpp"


#include "ConnectionHandler.hpp"
#include "CommandHandler.hpp"
#include "PlayerSession.hpp"



//================================================
// Constructor
//================================================

ClientConnection::ClientConnection(
    int id,
    CommandHandler& commandHandler)
:
m_id(id)
{

    m_player =
        std::make_shared<PlayerSession>(
            std::to_string(id));



    m_handler =
        std::make_unique<ConnectionHandler>(
            commandHandler,
            *m_player);

}



//================================================
// Destructor
//================================================

ClientConnection::~ClientConnection()
{
}



//================================================
// Send Command
//================================================

MoveResult ClientConnection::send(
    const std::string& message)
{

    if(!m_handler)
    {
        return
        {
            false,
            "connection_handler_missing"
        };
    }



    return m_handler->receive(
        message);
}



//================================================
// Deliver Snapshot
//================================================

void ClientConnection::deliverSnapshot(
    const GameSnapshot& snapshot)
{

    /*
        Snapshot represents immutable
        game information.

        A client receives a copy.

        No game logic exists here.
    */

    m_lastSnapshot =
        snapshot;

}



//================================================
// Get Last Snapshot
//================================================

const std::optional<GameSnapshot>&
ClientConnection::getLastSnapshot() const
{
    return m_lastSnapshot;
}



//================================================
// Get Id
//================================================

int ClientConnection::getId() const
{
    return m_id;
}



//================================================
// Get Player
//================================================

std::shared_ptr<PlayerSession>
ClientConnection::getPlayer()
{
    return m_player;
}



//================================================
// Get Player Session
//================================================

PlayerSession&
ClientConnection::getPlayerSession()
{
    return *m_player;
}