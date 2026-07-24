#pragma once

#include <string>
#include "PieceTypes.hpp"



/**
 * @brief Represents a connected player.
 *
 * This class belongs to the server core layer.
 *
 * Responsibilities:
 *  - Store player identity.
 *  - Store connection state.
 *  - Store assigned game side.
 *
 * Does NOT know:
 *  - GameEngine.
 *  - Board.
 *  - Network implementation.
 *  - Database.
 */
class PlayerSession
{
public:

    enum class ConnectionState
    {
        CONNECTED,
        DISCONNECTED
    };


public:

    /**
     * @brief Creates a player session.
     *
     * @param id Unique player identifier.
     */
    explicit PlayerSession(
        std::string id);


public:

    /**
     * @brief Returns player identifier.
     */
    const std::string& getId() const;


    /**
     * @brief Returns assigned side.
     */
    Side getSide() const;


    /**
     * @brief Assigns player side.
     */
    void setSide(
        Side side);


    /**
     * @brief Returns connection state.
     */
    ConnectionState getState() const;


    /**
     * @brief Updates connection state.
     */
    void setState(
        ConnectionState state);


private:

    /// Unique player identifier.
    std::string m_id;


    /// Player side in current game.
    Side m_side;


    /// Current connection state.
    ConnectionState m_state;
};