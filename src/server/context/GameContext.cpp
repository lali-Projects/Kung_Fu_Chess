#include "GameContext.hpp"


#include <stdexcept>


#include "Board.hpp"
#include "BoardInitializer.hpp"

#include "RuleEngine.hpp"

#include "RealTimeArbiter.hpp"

#include "GameEngine.hpp"

#include "GameController.hpp"

#include "GameSnapshotBuilder.hpp"





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
    // Real time
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
    // Snapshot
    //---------------------------------

    m_snapshotBuilder =
        std::make_unique<GameSnapshotBuilder>(
            *m_gameEngine,
            *m_controller);

}





GameContext::~GameContext() = default;






GameEngine&
GameContext::getEngine()
{
    if(!m_gameEngine)
        throw std::runtime_error(
            "GameEngine not available");


    return *m_gameEngine;
}





const GameEngine&
GameContext::getEngine() const
{
    if(!m_gameEngine)
        throw std::runtime_error(
            "GameEngine not available");


    return *m_gameEngine;
}





Board&
GameContext::getBoard()
{
    if(!m_board)
        throw std::runtime_error(
            "Board not available");


    return *m_board;
}





const Board&
GameContext::getBoard() const
{
    if(!m_board)
        throw std::runtime_error(
            "Board not available");


    return *m_board;
}





GameController&
GameContext::getController()
{
    if(!m_controller)
        throw std::runtime_error(
            "Controller not available");


    return *m_controller;
}





const GameController&
GameContext::getController() const
{
    if(!m_controller)
        throw std::runtime_error(
            "Controller not available");


    return *m_controller;
}





GameSnapshotBuilder&
GameContext::getSnapshotBuilder()
{
    if(!m_snapshotBuilder)
        throw std::runtime_error(
            "Snapshot builder not available");


    return *m_snapshotBuilder;
}





const GameSnapshotBuilder&
GameContext::getSnapshotBuilder() const
{
    if(!m_snapshotBuilder)
        throw std::runtime_error(
            "Snapshot builder not available");


    return *m_snapshotBuilder;
}






bool GameContext::hasController() const
{
    return m_controller != nullptr;
}



bool GameContext::hasSnapshotBuilder() const
{
    return m_snapshotBuilder != nullptr;
}