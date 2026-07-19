#pragma once
#include "Board.hpp"
#include "RuleEngine.hpp"
#include "GameConfig.hpp"
#include "RealTimeArbiter.hpp"
#include <string>
#include <memory>

class GameSnapshot;

/**
 * @struct MoveResult
 * @brief Encapsulates the result of a move request.
 */
struct MoveResult {
    bool success;       ///< Indicates if the move was successful.
    std::string reason; ///< Explanation if the move failed.
};

/**
 * @class GameEngine
 * @brief The core engine managing the game logic, state, and rules.
 */
class GameEngine {
private:
    Board& board;                     
    RuleEngine& ruleEngine;            
    RealTimeArbiter& realTimeArbiter; 
    int currentTimeMs;                
    bool isGameOver;             

public:
    /**
     * @brief Constructs a new GameEngine object.
     */
    GameEngine(Board& board, RuleEngine& ruleEngine, RealTimeArbiter& arbiter)
        : board(board), ruleEngine(ruleEngine), realTimeArbiter(arbiter),
          currentTimeMs(GameConfig::INITIAL_TIME_MS), isGameOver(false) {}

    /**
     * @brief Gets the game board.
     * @return Const reference to the board.
     */
    const Board& getBoard() const { return board; }

    /**
     * @brief Gets the current elapsed time.
     * @return Current time in milliseconds.
     */
    int getCurrentTime() const { return currentTimeMs; }
    
    /**
     * @brief Requests a standard move.
     * @param from Source position.
     * @param to Target position.
     * @return The result of the move request.
     */
    MoveResult requestMove(const Position& from, const Position& to);

    /**
     * @brief Requests a jump move.
     * @param position Source position.
     * @return The result of the jump request.
     */
    MoveResult requestJump(const Position& position);

    /**
     * @brief Advances the game time and notifies the arbiter.
     * @param milliseconds Time increment in milliseconds.
     */
    void wait(int milliseconds) {
        currentTimeMs += milliseconds;
        realTimeArbiter.advanceTime(currentTimeMs, *this);
    }

    /**
     * @brief Sets the game state to over.
     */
    void signalGameOver() {
        isGameOver = true;
    }

    /**
     * @brief Checks if the game has ended.
     * @return True if game is over, false otherwise.
     */
    bool gameOver() const {
        return isGameOver;
    }
   
    /**
     * @brief Generates a snapshot of the current game state.
     * @return A GameSnapshot object representing the current state.
     */
    GameSnapshot getSnapshot() const;

    /**
     * @brief Retrieves the animation start time for a specific piece.
     * @param pieceId The ID of the piece.
     * @return The start time in milliseconds.
     */
    int getAnimationStartTime(int pieceId) const;
};