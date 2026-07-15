#include "PieceSnapshot.hpp"

PieceSnapshot::PieceSnapshot(
    int id,
    Side side,
    PieceType type,
    PieceState state,
    const Position& position)
    :
    id(id),
    side(side),
    type(type),
    state(state),
    position(position)
{
}

int PieceSnapshot::getId() const
{
    return id;
}

Side PieceSnapshot::getSide() const
{
    return side;
}

PieceType PieceSnapshot::getType() const
{
    return type;
}

PieceState PieceSnapshot::getState() const
{
    return state;
}

const Position& PieceSnapshot::getPosition() const
{
    return position;
}

std::string PieceSnapshot::toCanonicalString() const
{
    std::string sideChar =
        (side == Side::WHITE) ? "w" : "b";

    return sideChar + typeToString(type);
}