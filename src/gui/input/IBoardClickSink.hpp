#pragma once

#include "Position.hpp"


/** Presentation input boundary for a logical board click. */
class IBoardClickSink
{
public:
    virtual ~IBoardClickSink() = default;
    virtual void click(const Position& position) = 0;
};
