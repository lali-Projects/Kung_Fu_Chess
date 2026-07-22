#pragma once

#include <memory>
#include <string>
#include <vector>

#include "PlayerSession.hpp"
#include "EventBus.hpp"

#include "GameController.hpp"


/**
 * @brief Represents one active game session.
 *
 * GameSession is the bridge between the server layer
 * and the existing game logic.
 *
 * Communication flow:
 *
 * Server
 *   |
 *   v
 * GameSession
 *   |
 *   v
 * GameController
 *   |
 *   v
 * GameEngine
 *
 *
 * Responsibilities:
 *  - Manage players participating in this game.
 *  - Forward player commands to GameController.
 *  - Publish game events.
 *
 * Does NOT know:
 *  - MouseInput.
 *  - Layout.
 *  - Pixel coordinates.
 *  - Game rules.
 *  - Board manipulation.
 */
class GameSession
{
public:

    /**
     * @brief Creates a game session.
     *
     * @param id Unique game identifier.
     * @param controller Controller responsible for game logic access.
     * @param eventBus Event system used for notifications.
     */
    GameSession(
        std::string id,
        std::unique_ptr<GameController> controller,
        EventBus& eventBus);


    ~GameSession() = default;


    GameSession(const GameSession&) = delete;
    GameSession& operator=(const GameSession&) = delete;

    GameSession(GameSession&&) = delete;
    GameSession& operator=(GameSession&&) = delete;



public:

    /**
     * @brief Returns game session identifier.
     */
    const std::string& getId() const;



    /**
     * @brief Adds a player to this game.
     *
     * Maximum two players:
     * WHITE
     * BLACK
     */
    bool addPlayer(
        std::shared_ptr<PlayerSession> player);



    /**
     * @brief Handles a board position command.
     *
     * The input is already converted from
     * screen coordinates to board coordinates.
     *
     * Example:
     *
     * MouseInput:
     *      pixel(x,y)
     *          |
     *          v
     *      Position
     *
     * Server:
     *      Network command
     *          |
     *          v
     *      Position
     *
     */
    MoveResult handleClick(
        const Position& position);



private:

    /**
     * @brief Unique identifier of this game.
     */
    std::string m_id;



    /**
     * @brief Event communication system.
     */
    EventBus& m_eventBus;



    /**
     * @brief Entry point into game logic.
     *
     * GameSession does not access GameEngine directly.
     */
    std::unique_ptr<GameController> m_controller;



    /**
     * @brief Players connected to this game.
     */
    std::vector<
        std::shared_ptr<PlayerSession>>
        m_players;
};