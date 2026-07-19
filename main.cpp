#include <iostream>

#include "Board.hpp"
#include "BoardInitializer.hpp"

#include "RuleEngine.hpp"
#include "RealTimeArbiter.hpp"
#include "GameEngine.hpp"

#include "GameController.hpp"
#include "MouseInput.hpp"

#include "GameSnapshotBuilder.hpp"

#include "GuiConfig.hpp"
#include "Layout.hpp"

#include "TextureManager.hpp"
#include "GameRenderer.hpp"
#include "Window.hpp"
#include "GameLoop.hpp"

int main()
{
    std::cout
        << "==============================\n"
        << " KUNG FU CHESS GUI TEST\n"
        << "==============================\n\n";

    //---------------------------------
    // Logic
    //---------------------------------

    Board board(8, 8);

    BoardInitializer::setupInitialPosition(board);

    RuleEngine ruleEngine;

    RealTimeArbiter arbiter(board);

    GameEngine engine(
        board,
        ruleEngine,
        arbiter);

    //---------------------------------
    // Layout
    //---------------------------------

    Layout layout(
        GuiConfig::WINDOW_WIDTH,
        GuiConfig::WINDOW_HEIGHT,
        8,
        8);

    //---------------------------------
    // Controller
    //---------------------------------

    GameController controller(
        board,
        engine);

    MouseInput mouseInput(
        controller,
        layout);

    //---------------------------------
    // Snapshot
    //---------------------------------

    GameSnapshotBuilder snapshotBuilder(
        engine,
        controller);

    //---------------------------------
    // GUI
    //---------------------------------

    TextureManager textureManager;

    try
    {
        textureManager.loadBoardTexture(
            GuiConfig::BOARD_TEXTURE_PATH);

        //
        // במקום loadAllPieceTextures(...)
        //
        textureManager.loadAllPieceAnimations(
            layout.getCellSize());

        std::cout
            << "[OK] All animations loaded\n";
    }
    catch(const std::exception& e)
    {
        std::cout
            << e.what()
            << std::endl;

        return 1;
    }

    GameRenderer renderer(
        layout,
        textureManager);

    Window window(
        GuiConfig::WINDOW_TITLE);

    //---------------------------------
    // Mouse
    //---------------------------------

    window.setMouseCallback(
        [&](int x, int y)
        {
            mouseInput.click(x, y);
        });

    //---------------------------------
    // Game Loop
    //---------------------------------

    GameLoop loop(
        engine,
        snapshotBuilder,
        renderer,
        window,
        GuiConfig::WINDOW_WIDTH,
        GuiConfig::WINDOW_HEIGHT,
        GuiConfig::FPS);

    //---------------------------------
    // Run
    //---------------------------------

    loop.run();

    return 0;
}
