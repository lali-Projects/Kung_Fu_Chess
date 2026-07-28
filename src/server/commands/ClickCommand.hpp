#pragma once

#include "Position.hpp"

/**
 * @brief Represents a click action on a specific board position.
 *
 * Stores the board position selected by the client
 * and provides read-only access to it.
 */
class ClickCommand
{
public:
    
    explicit ClickCommand(const Position& position);

    const Position& getPosition() const;

private:
    
    Position m_position;
};