//  #include <iostream>
// #include <exception>


// #include "Board.hpp"
// #include "BoardInitializer.hpp"

// #include "RuleEngine.hpp"
// #include "RealTimeArbiter.hpp"
// #include "GameEngine.hpp"

// #include "GameController.hpp"
// #include "MouseInput.hpp"

// #include "GameSnapshotBuilder.hpp"

// #include "GuiConfig.hpp"
// #include "Layout.hpp"

// #include "TextureManager.hpp"
// #include "GameRenderer.hpp"
// #include "Window.hpp"
// #include "GameLoop.hpp"



// int main()
// {
//     try
//     {
//         Board board(
//             8,
//             8);


//         BoardInitializer::setupInitialPosition(
//             board);



//         RuleEngine ruleEngine;



//         RealTimeArbiter arbiter(
//             board);



//         GameEngine engine(
//             board,
//             ruleEngine,
//             arbiter);



//         Layout layout(
//             GuiConfig::WINDOW_WIDTH,
//             GuiConfig::WINDOW_HEIGHT,
//             8,
//             8);



//         GameController controller(
//             board,
//             engine);



//         MouseInput mouseInput(
//             controller,
//             layout);



//         GameSnapshotBuilder snapshotBuilder(
//             engine,
//             controller);



//         TextureManager textureManager;


//         textureManager.loadBoardTexture(
//             GuiConfig::BOARD_TEXTURE_PATH);


//         textureManager.loadAllPieceAnimations(
//             layout.getCellSize());



//         GameRenderer renderer(
//             layout,
//             textureManager);



//         Window window(
//             GuiConfig::WINDOW_TITLE);



//         window.setMouseCallback(
//             [&](int x, int y)
//             {
//                 mouseInput.click(
//                     x,
//                     y);
//             });



//         GameLoop loop(
//             engine,
//             snapshotBuilder,
//             renderer,
//             window,
//             GuiConfig::WINDOW_WIDTH,
//             GuiConfig::WINDOW_HEIGHT,
//             GuiConfig::FPS);



//         loop.run();


//         return 0;
//     }
//     catch(const std::exception& e)
//     {
//         std::cerr
//             << "[FATAL ERROR] "
//             << e.what()
//             << std::endl;

//         return 1;
//     }
//     catch(...)
//     {
//         std::cerr
//             << "[FATAL ERROR] Unknown exception"
//             << std::endl;

//         return 1;
//     }
// }

#include <iostream>

#include "Application.hpp"



int main()
{
    std::cout
        << "===== Server Flow Test ====="
        << std::endl;



    Application app;



    app.start();



    /*
        First command:

        Select white pawn

        Initial position:

        (6,0)
    */

    MoveResult selectResult =
        app.sendCommand(
            "CLICK 6 0");



    std::cout
        << "\nSelection:"
        << std::endl;


    std::cout
        << "Result: "
        << (selectResult.success
                ? "SUCCESS"
                : "FAILED")
        << std::endl;


    std::cout
        << "Reason: "
        << selectResult.reason
        << std::endl;



    /*
        Second command:

        Move pawn

        From:

        (6,0)

        To:

        (5,0)
    */

    MoveResult moveResult =
        app.sendCommand(
            "CLICK 5 0");



    std::cout
        << "\nMove:"
        << std::endl;


    std::cout
        << "Result: "
        << (moveResult.success
                ? "SUCCESS"
                : "FAILED")
        << std::endl;


    std::cout
        << "Reason: "
        << moveResult.reason
        << std::endl;



    app.stop();



    std::cout
        << "\n===== Finished ====="
        << std::endl;



    return 0;
}