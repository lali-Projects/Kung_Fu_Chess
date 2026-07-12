#pragma once
#include "Position.hpp"
#include <string>

enum class Side { WHITE, BLACK };
enum class PieceType { KING, QUEEN, ROOK, BISHOP, KNIGHT, PAWN };
enum class PieceState { IDLE, MOVING, CAPTURED, AIRBORNE }; 

inline std::string typeToString(PieceType type) {
    switch (type) 
    {
        case PieceType::KING:   return "K";
        case PieceType::QUEEN:  return "Q";
        case PieceType::ROOK:   return "R";
        case PieceType::BISHOP: return "B";
        case PieceType::KNIGHT: return "N";
        case PieceType::PAWN:   return "P";
    }
    return "";
}

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