#pragma once

#include <string>
#include "PieceTypes.hpp"
#include "Position.hpp"

class PieceSnapshot
{
private:
    int id;
    Side side;
    PieceType type;
    PieceState state;
    Position position;

public:
    PieceSnapshot(
        int id,
        Side side,
        PieceType type,
        PieceState state,
        const Position& position);

    int getId() const;
    Side getSide() const;
    PieceType getType() const;
    PieceState getState() const;
    const Position& getPosition() const;
    /**
 * @brief מחזיר את הייצוג הקנוני של הכלי (למשל wK או bP).
 */
std::string toCanonicalString() const;  
};