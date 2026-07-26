#pragma once


#include <memory>



class Board;
class RuleEngine;
class RealTimeArbiter;
class GameEngine;

class GameController;
class GameSnapshotBuilder;




class GameContext
{

public:


    GameContext();


    ~GameContext();



    GameContext(
        const GameContext&) = delete;



    GameContext& operator=(
        const GameContext&) = delete;




public:


//=================================
// Core
//=================================


    GameEngine& getEngine();

    const GameEngine& getEngine() const;



    Board& getBoard();

    const Board& getBoard() const;



    RuleEngine& getRuleEngine();

    const RuleEngine& getRuleEngine() const;



    RealTimeArbiter& getRealTimeArbiter();

    const RealTimeArbiter& getRealTimeArbiter() const;





//=================================
// Services
//=================================


    GameController& getController();

    const GameController& getController() const;



    GameSnapshotBuilder& getSnapshotBuilder();

    const GameSnapshotBuilder& getSnapshotBuilder() const;





//=================================
// Validation
//=================================


    bool hasBoard() const;

    bool hasEngine() const;

    bool hasController() const;

    bool hasSnapshotBuilder() const;

bool hasRuleEngine() const;

bool hasRealTimeArbiter() const;


private:


    std::unique_ptr<Board>
        m_board;



    std::unique_ptr<RuleEngine>
        m_ruleEngine;



    std::unique_ptr<RealTimeArbiter>
        m_realTimeArbiter;



    std::unique_ptr<GameEngine>
        m_gameEngine;



    std::unique_ptr<GameController>
        m_controller;



    std::unique_ptr<GameSnapshotBuilder>
        m_snapshotBuilder;

};