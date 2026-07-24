#pragma once

#include "Position.hpp"


/**
 * @brief Represents a click command from a player.
 *
 * This class contains only input data.
 *
 * Flow:
 *
 * Client
 *    |
 *    v
 * ClickCommand
 *    |
 *    v
 * GameSession
 *    |
 *    v
 * GameController
 *    |
 *    v
 * GameEngine
 *
 *
 * This class does NOT:
 *  - Validate moves.
 *  - Decide between MOVE and JUMP.
 *  - Access game logic.
 */
class ClickCommand
{
public:

    /**
     * @brief Creates a click command.
     *
     * @param position Board position clicked by player.
     */
    explicit ClickCommand(
        const Position& position);



    /**
     * @brief Returns clicked board position.
     */
    const Position& getPosition() const;



private:

    /**
     * @brief Position on the board.
     *
     * Already converted from pixels
     * by the client side.
     */
    Position m_position;
};