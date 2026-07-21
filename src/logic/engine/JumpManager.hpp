#pragma once

#include <optional>
#include <memory>

#include "Jump.hpp"
#include "Position.hpp"

/**
 * @class JumpManager
 * @brief Manages active jump actions and tracks their completion over time.
 *
 * Responsible only for jump lifecycle management.
 */
class JumpManager
{
private:
    std::optional<Jump> activeJump;

    std::shared_ptr<Piece> finishedJumpPiece;

public:
    bool startJump(std::shared_ptr<Piece> piece, int startTime, int duration);

    bool update(int currentTime);

    bool hasActiveJump() const;

    bool hasActiveJumpFor(std::shared_ptr<Piece> piece) const;

    bool isJumpAt(const Position& pos) const;

    const Jump* getActiveJump() const;

    int getStartTime(std::shared_ptr<Piece> piece) const;
    
    std::shared_ptr<Piece> getFinishedJumpPiece();
};