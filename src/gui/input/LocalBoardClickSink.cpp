#include "LocalBoardClickSink.hpp"

#include <iostream>

#include "GameController.hpp"


LocalBoardClickSink::LocalBoardClickSink(
    GameController& controller)
    : m_controller(controller)
{
}


void LocalBoardClickSink::click(
    const Position& position)
{
    const MoveResult result =
        m_controller.click(position);
    std::cout
        << "Local board click: "
        << (result.success ? "success" : "failure")
        << " ("
        << result.reason
        << ")\n";
}
