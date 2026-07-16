#pragma once

#include "GameEngine.hpp"
#include "GameSnapshotBuilder.hpp"
#include "GameRenderer.hpp"
#include "Window.hpp"
#include "Img.hpp"

#include <chrono>

class GameLoop
{
private:

    GameEngine& gameEngine;

    GameSnapshotBuilder& snapshotBuilder;

    GameRenderer& renderer;

    Window& window;

    Img canvas;

    bool running;

    int fps;

public:

    GameLoop(
        GameEngine& engine,
        GameSnapshotBuilder& snapshotBuilder,
        GameRenderer& renderer,
        Window& window,
        int width,
        int height,
        int fps = 60);

    void run();

    void stop();
};