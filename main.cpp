#include <iostream>
#include <exception>


// Logic
#include "Board.hpp"
#include "BoardInitializer.hpp"

#include "RuleEngine.hpp"
#include "RealTimeArbiter.hpp"
#include "GameEngine.hpp"


// Input
#include "GameController.hpp"
#include "MouseInput.hpp"


// Snapshot
#include "GameSnapshotBuilder.hpp"


// GUI
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
    // Board
    //---------------------------------

    Board board(
        8,
        8);



    BoardInitializer::setupInitialPosition(
        board);




    //---------------------------------
    // Rules
    //---------------------------------

    RuleEngine ruleEngine;




    //---------------------------------
    // Real Time System
    //---------------------------------
    //
    // אחראי על:
    // - Motion
    // - Jump
    // - Rest
    // - זמן אמת
    //
    // MoveExecutor מנוהל פנימית
    //

    RealTimeArbiter arbiter(
        board);





    //---------------------------------
    // Game Engine
    //---------------------------------

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
    // Snapshot Builder
    //---------------------------------

    GameSnapshotBuilder snapshotBuilder(
        engine,
        controller);





    //---------------------------------
    // Texture Loading
    //---------------------------------

    TextureManager textureManager;


    try
    {

        textureManager.loadBoardTexture(
            GuiConfig::BOARD_TEXTURE_PATH);



        textureManager.loadAllPieceAnimations(
            layout.getCellSize());



        std::cout
            << "[OK] Textures and animations loaded\n";

    }

    catch(const std::exception& e)
    {

        std::cerr
            << "[ERROR] "
            << e.what()
            << std::endl;


        return 1;

    }





    //---------------------------------
    // Renderer
    //---------------------------------

    GameRenderer renderer(
        layout,
        textureManager);





    //---------------------------------
    // Window
    //---------------------------------

    Window window(
        GuiConfig::WINDOW_TITLE);





    //---------------------------------
    // Mouse Events
    //---------------------------------

    window.setMouseCallback(
        [&](int x, int y)
        {

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