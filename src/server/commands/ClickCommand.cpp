#include "ClickCommand.hpp"


//================================================
// Constructor
//================================================

ClickCommand::ClickCommand(
    const Position& position)
    :
    m_position(position)
{
}



//================================================
// Get Position
//================================================

const Position& ClickCommand::getPosition() const
{
    return m_position;
}