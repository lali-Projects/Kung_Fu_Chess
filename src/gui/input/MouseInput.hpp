#pragma once

#include "GameController.hpp"
#include "Layout.hpp"


class MouseInput
{

private:

    GameController& controller;
    Layout& layout;

public:

    MouseInput(GameController& controller, Layout& layout);

    void click(int x, int y);

};