#pragma once

#include <vector>
#include <memory>

#include "Rest.hpp"
#include "Piece.hpp"

/**
 * @class RestManager
 * @brief Manages piece rest states in real-time.
 *
 * Responsible for:
 * - Creating Rests
 * - Storing active Rests
 * - Ending Rests
 * - Returning a piece to its next state
 *
 * Does not:
 * - Manage time
 * - Modify the board
 */
class RestManager
{
private:
    std::vector<Rest> activeRests;

    int getRestDuration(PieceState restState) const;

public:
    bool startRest(std::shared_ptr<Piece> piece, PieceState restState, PieceState nextState, int startTime);

    void update(int currentTime);
    
    bool hasActiveRestFor(std::shared_ptr<Piece> piece) const;

    int getRestStartTime(std::shared_ptr<Piece> piece) const;

    int getRestFinishTime(std::shared_ptr<Piece> piece) const;
};