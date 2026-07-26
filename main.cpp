 #include <iostream>
#include <exception>


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
    try
    {
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



        GameController controller(
            board,
            engine);



        MouseInput mouseInput(
            controller,
            layout);



        GameSnapshotBuilder snapshotBuilder(
            engine,
            controller);



        TextureManager textureManager;


        textureManager.loadBoardTexture(
            GuiConfig::BOARD_TEXTURE_PATH);


        textureManager.loadAllPieceAnimations(
            layout.getCellSize());



        GameRenderer renderer(
            layout,
            textureManager);



        Window window(
            GuiConfig::WINDOW_TITLE);



        window.setMouseCallback(
            [&](int x, int y)
            {
                mouseInput.click(
                    x,
                    y);
            });



        GameLoop loop(
            engine,
            snapshotBuilder,
            renderer,
            window,
            GuiConfig::WINDOW_WIDTH,
            GuiConfig::WINDOW_HEIGHT,
            GuiConfig::FPS);



        loop.run();


        return 0;
    }
    catch(const std::exception& e)
    {
        std::cerr
            << "[FATAL ERROR] "
            << e.what()
            << std::endl;

        return 1;
    }
    catch(...)
    {
        std::cerr
            << "[FATAL ERROR] Unknown exception"
            << std::endl;

        return 1;
    }
}



// #include "Application.hpp"

// #include <iostream>

// #include <ixwebsocket/IXNetSystem.h>


// int main()
// {
//     try
//     {
//         ix::initNetSystem();


//         Application app;


//         app.start();


//         std::cout
//             << "[MAIN] Application running\n";


//         std::cout
//             << "Press ENTER to stop...\n";


//         std::cin.get();


//         app.stop();


//         ix::uninitNetSystem();
//     }
//     catch(const std::exception& e)
//     {
//         std::cerr
//             << "[MAIN ERROR] "
//             << e.what()
//             << "\n";


//         ix::uninitNetSystem();

//         return 1;
//     }


//     return 0;
// }