#pragma once

#include "IBoardClickSink.hpp"


class GameController;


/** Preserves the existing in-process input path for local tools/tests. */
class LocalBoardClickSink : public IBoardClickSink
{
public:
    explicit LocalBoardClickSink(GameController& controller);
    void click(const Position& position) override;

private:
    GameController& m_controller;
};
