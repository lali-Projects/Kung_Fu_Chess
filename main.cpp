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

    Board board(
        8,
        8);


    BoardInitializer::setupInitialPosition(
        board);


    RuleEngine ruleEngine;


    RealTimeArbiter arbiter(
        board);


    GameEngine engine(
        board,
        ruleEngine,
        arbiter);

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


MouseInput mouseInput(controller, layout);



    //---------------------------------
    // Snapshot Builder
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


        textureManager.loadAllPieceTextures(
            layout.getCellSize());


        std::cout
            << "[OK] All textures loaded\n";
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
    // Mouse connection
    //---------------------------------

    window.setMouseCallback(
        [&](int x, int y)
        {
            std::cout
                << "Mouse click: "
                << x
                << ", "
                << y
                << std::endl;


            mouseInput.click(
                x,
                y);
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