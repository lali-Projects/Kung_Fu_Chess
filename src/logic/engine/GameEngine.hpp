#pragma once

#include <string>
#include <memory>

#include "Board.hpp"
#include "RuleEngine.hpp"
#include "GameConfig.hpp"
#include "RealTimeArbiter.hpp"

#include "GameOverHandler.hpp"
#include "PromotionHandler.hpp"


class GameSnapshot;

/**
 * @struct MoveResult
 * @brief Result of requesting a move.
 */
struct MoveResult
{
    bool success;
    std::string reason;
};

class GameEngine
{
private:
    Board& board;
    RuleEngine& ruleEngine;
    RealTimeArbiter& realTimeArbiter;
    GameOverHandler gameOverHandler;
    PromotionHandler promotionHandler;

    int currentTimeMs;
    bool isGameOver;

   std::shared_ptr<Piece> getPieceById(int pieceId) const;
public:
    GameEngine(Board& board, RuleEngine& ruleEngine, RealTimeArbiter& arbiter);

    //---------------------------------
    // Requests
    //---------------------------------
    MoveResult requestMove(const Position& from, const Position& to);

    MoveResult requestJump(const Position& position);

    //---------------------------------
    // Time
    //---------------------------------
    void wait(int milliseconds);

    //---------------------------------
    // Execution Result
    //---------------------------------
    void handleMoveExecutionResult(const MoveExecutionResult& result);

    //---------------------------------
    // Game State
    //---------------------------------
    void signalGameOver();

    bool gameOver() const;

    //---------------------------------
    // Access
    //---------------------------------
    const Board& getBoard() const;

    int getCurrentTime() const;

    //---------------------------------
    // Snapshot
    //---------------------------------
    GameSnapshot getSnapshot() const;

int getAnimationStartTime(int pieceId) const;};