#include "GameContext.hpp"


#include <stdexcept>


#include "Board.hpp"
#include "BoardInitializer.hpp"

#include "RuleEngine.hpp"

#include "RealTimeArbiter.hpp"

#include "GameEngine.hpp"

#include "GameController.hpp"

#include "GameSnapshotBuilder.hpp"




//================================================
// Constructor
//================================================

GameContext::GameContext()
{

    //---------------------------------
    // Board
    //---------------------------------

    m_board =
        std::make_unique<Board>(
            8,
            8);



    BoardInitializer::setupInitialPosition(
        *m_board);





    //---------------------------------
    // Rules
    //---------------------------------

    m_ruleEngine =
        std::make_unique<RuleEngine>();







    //---------------------------------
    // Real Time
    //---------------------------------

    m_realTimeArbiter =
        std::make_unique<RealTimeArbiter>(
            *m_board);







    //---------------------------------
    // Engine
    //---------------------------------

    m_gameEngine =
        std::make_unique<GameEngine>(
            *m_board,
            *m_ruleEngine,
            *m_realTimeArbiter);








    //---------------------------------
    // Controller
    //---------------------------------

    m_controller =
        std::make_unique<GameController>(
            *m_board,
            *m_gameEngine);








    //---------------------------------
    // Snapshot Builder
    //---------------------------------
    //
    // Updated:
    // Requires Engine + Controller
    //
    //---------------------------------

    m_snapshotBuilder =
        std::make_unique<GameSnapshotBuilder>(
            *m_gameEngine,
            *m_controller);

}







//================================================
// Destructor
//================================================

GameContext::~GameContext() = default;







//================================================
// Engine
//================================================

GameEngine&
GameContext::getEngine()
{

    if(!m_gameEngine)
    {
        throw std::runtime_error(
            "GameEngine not available");
    }


    return *m_gameEngine;
}






const GameEngine&
GameContext::getEngine() const
{

    if(!m_gameEngine)
    {
        throw std::runtime_error(
            "GameEngine not available");
    }


    return *m_gameEngine;
}







//================================================
// Board
//================================================

Board&
GameContext::getBoard()
{

    if(!m_board)
    {
        throw std::runtime_error(
            "Board not available");
    }


    return *m_board;
}






const Board&
GameContext::getBoard() const
{

    if(!m_board)
    {
        throw std::runtime_error(
            "Board not available");
    }


    return *m_board;
}







//================================================
// Rule Engine
//================================================

RuleEngine&
GameContext::getRuleEngine()
{

    if(!m_ruleEngine)
    {
        throw std::runtime_error(
            "RuleEngine not available");
    }


    return *m_ruleEngine;
}






const RuleEngine&
GameContext::getRuleEngine() const
{

    if(!m_ruleEngine)
    {
        throw std::runtime_error(
            "RuleEngine not available");
    }


    return *m_ruleEngine;
}







//================================================
// Real Time Arbiter
//================================================

RealTimeArbiter&
GameContext::getRealTimeArbiter()
{

    if(!m_realTimeArbiter)
    {
        throw std::runtime_error(
            "RealTimeArbiter not available");
    }


    return *m_realTimeArbiter;
}






const RealTimeArbiter&
GameContext::getRealTimeArbiter() const
{

    if(!m_realTimeArbiter)
    {
        throw std::runtime_error(
            "RealTimeArbiter not available");
    }


    return *m_realTimeArbiter;
}







//================================================
// Controller
//================================================

GameController&
GameContext::getController()
{

    if(!m_controller)
    {
        throw std::runtime_error(
            "Controller not available");
    }


    return *m_controller;
}






const GameController&
GameContext::getController() const
{

    if(!m_controller)
    {
        throw std::runtime_error(
            "Controller not available");
    }


    return *m_controller;
}







//================================================
// Snapshot Builder
//================================================

GameSnapshotBuilder&
GameContext::getSnapshotBuilder()
{

    if(!m_snapshotBuilder)
    {
        throw std::runtime_error(
            "SnapshotBuilder not available");
    }


    return *m_snapshotBuilder;
}






const GameSnapshotBuilder&
GameContext::getSnapshotBuilder() const
{

    if(!m_snapshotBuilder)
    {
        throw std::runtime_error(
            "SnapshotBuilder not available");
    }


    return *m_snapshotBuilder;
}







//================================================
// Validation
//================================================

bool GameContext::hasBoard() const
{
    return m_board != nullptr;
}




bool GameContext::hasEngine() const
{
    return m_gameEngine != nullptr;
}




bool GameContext::hasController() const
{
    return m_controller != nullptr;
}




bool GameContext::hasSnapshotBuilder() const
{
    return m_snapshotBuilder != nullptr;
}




bool GameContext::hasRuleEngine() const
{
    return m_ruleEngine != nullptr;
}




bool GameContext::hasRealTimeArbiter() const
{
    return m_realTimeArbiter != nullptr;
}