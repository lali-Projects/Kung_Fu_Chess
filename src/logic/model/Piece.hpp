#pragma once
#include "Position.hpp"
#include <string>
#include "PieceTypes.hpp"

class Piece {
private:
    int id;
    Side side;
    PieceType type;
    Position pos;
    PieceState state; 

public:
    Piece(int id, Side s, PieceType t, Position p) 
        : id(id), side(s), type(t), pos(p), state(PieceState::IDLE) {}

    virtual ~Piece() = default;

    // Getters
    int getId() const { return id; }
    Side getSide() const { return side; }
    PieceType getType() const { return type; }
    Position getPosition() const { return pos; }
    PieceState getState() const { return state; } 

    // Setters
    void setPosition(const Position& newPos) { pos = newPos; }
    void setState(PieceState newState) { state = newState; } 
    void setType(PieceType newType) { type = newType; }
    
    virtual std::string toCanonicalString() const {
        std::string sideChar = (getSide() == Side::WHITE) ? "w" : "b";
        return sideChar + typeToString(getType());
    }
};