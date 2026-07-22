// #include <iostream>
// #include <exception>

// // Logic
// #include "Board.hpp"
// #include "BoardInitializer.hpp"

// #include "RuleEngine.hpp"
// #include "RealTimeArbiter.hpp"
// #include "GameEngine.hpp"

// // Input
// #include "GameController.hpp"
// #include "MouseInput.hpp"

// // Snapshot
// #include "GameSnapshotBuilder.hpp"

// // GUI
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
//         std::cout
//             << "==============================\n"
//             << " KUNG FU CHESS GUI TEST\n"
//             << "==============================\n\n";

//         //---------------------------------
//         // Board
//         //---------------------------------

//         Board board(
//             8,
//             8);

//         BoardInitializer::setupInitialPosition(
//             board);

//         //---------------------------------
//         // Rules
//         //---------------------------------

//         RuleEngine ruleEngine;

//         //---------------------------------
//         // Real Time System
//         //---------------------------------
//         //
//         // אחראי על:
//         // - Motion
//         // - Jump
//         // - Rest
//         // - זמן אמת
//         //
//         // MoveExecutor מנוהל פנימית
//         //

//         RealTimeArbiter arbiter(
//             board);

//         //---------------------------------
//         // Game Engine
//         //---------------------------------

//         GameEngine engine(
//             board,
//             ruleEngine,
//             arbiter);

//         //---------------------------------
//         // Layout
//         //---------------------------------

//         Layout layout(
//             GuiConfig::WINDOW_WIDTH,
//             GuiConfig::WINDOW_HEIGHT,
//             8,
//             8);

//         //---------------------------------
//         // Controller
//         //---------------------------------

//         GameController controller(
//             board,
//             engine);

//         MouseInput mouseInput(
//             controller,
//             layout);

//         //---------------------------------
//         // Snapshot Builder
//         //---------------------------------

//         GameSnapshotBuilder snapshotBuilder(
//             engine,
//             controller);

//         //---------------------------------
//         // Texture Loading
//         //---------------------------------

//         TextureManager textureManager;

//         try
//         {
//             textureManager.loadBoardTexture(
//                 GuiConfig::BOARD_TEXTURE_PATH);

//             textureManager.loadAllPieceAnimations(
//                 layout.getCellSize());

//             std::cout
//                 << "[OK] Textures and animations loaded\n";
//         }
//         catch(const std::exception& e)
//         {
//             std::cerr
//                 << "[ERROR] Texture loading failed: "
//                 << e.what()
//                 << std::endl;

//             return 1;
//         }

//         //---------------------------------
//         // Renderer
//         //---------------------------------

//         GameRenderer renderer(
//             layout,
//             textureManager);

//         //---------------------------------
//         // Window
//         //---------------------------------

//         Window window(
//             GuiConfig::WINDOW_TITLE);

//         //---------------------------------
//         // Mouse Events
//         //---------------------------------

//         window.setMouseCallback(
//             [&](int x, int y)
//             {
//                 mouseInput.click(
//                     x,
//                     y);
//             });

//         //---------------------------------
//         // Game Loop
//         //---------------------------------

//         GameLoop loop(
//             engine,
//             snapshotBuilder,
//             renderer,
//             window,
//             GuiConfig::WINDOW_WIDTH,
//             GuiConfig::WINDOW_HEIGHT,
//             GuiConfig::FPS);

//         //---------------------------------
//         // Run
//         //---------------------------------

//         loop.run();

//         return 0;
//     }
//     catch(const std::exception& e)
//     {
//         std::cerr
//             << "[FATAL ERROR] Unhandled exception in main: "
//             << e.what()
//             << std::endl;
//         return 1;
//     }
//     catch(...)
//     {
//         std::cerr
//             << "[FATAL ERROR] Unknown exception occurred"
//             << std::endl;
//         return 1;
//     }
// }

#include <iostream>
#include <memory>


#include "GameSession.hpp"
#include "CommandHandler.hpp"
#include "ClickCommand.hpp"

#include "EventBus.hpp"


// Game logic
#include "Board.hpp"
#include "RuleEngine.hpp"
#include "RealTimeArbiter.hpp"
#include "GameEngine.hpp"
#include "GameController.hpp"



int main()
{
    std::cout
        << "===== Server Flow Test ====="
        << std::endl;



    /*
        1.
        Infrastructure
    */

    EventBus eventBus;



    /*
        2.
        Create game logic components


        Dependency order:

        Board
          |
          +--> RuleEngine
          |
          +--> RealTimeArbiter
                    |
                    v
              GameEngine
    */


    Board board(
        8,
        8);



    RuleEngine ruleEngine;



    RealTimeArbiter arbiter(
        board);



    GameEngine engine(
        board,
        ruleEngine,
        arbiter);



    /*
        Controller is the only gateway
        into the game logic.

        Server does NOT access
        GameEngine directly.
    */

    auto controller =
        std::make_unique<GameController>(
            board,
            engine);



    /*
        3.
        Create game session

        GameSession knows only:

        GameController
        EventBus

    */

    GameSession session(
        "game_001",
        std::move(controller),
        eventBus);



    /*
        4.
        Create command handler
    */

    CommandHandler handler(
        session);



    /*
        5.
        Simulate client command


        In a real server:

        Network JSON

        {
            row:3,
            col:3
        }

        becomes:

        Position(3,3)

        then:

        ClickCommand
    */


    Position clickedPosition(
        3,
        3);



    ClickCommand command(
        clickedPosition);



    /*
        6.
        Execute server flow:

        ClickCommand
              |
              v
        CommandHandler
              |
              v
        GameSession
              |
              v
        GameController
              |
              v
        GameEngine
    */


    MoveResult result =
        handler.handle(command);



    /*
        7.
        Display result
    */

    std::cout
        << "Result: "
        << (result.success ? "SUCCESS" : "FAILED")
        << std::endl;


    std::cout
        << "Reason: "
        << result.reason
        << std::endl;



    std::cout
        << "===== Finished ====="
        << std::endl;



    return 0;
}