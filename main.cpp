//  #include <iostream>
// #include <exception>


// #include "Board.hpp"
// #include "BoardInitializer.hpp"

// #include "RuleEngine.hpp"
// #include "RealTimeArbiter.hpp"
 #include "GameEngine.hpp"

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

#include "Server.hpp"
#include "ConnectionManager.hpp"
#include "ClientConnection.hpp"

#include "NetworkMessage.hpp"



int main()
{

    std::cout
        << "============================\n"
        << " Server Integration Test\n"
        << "============================\n\n";



    //---------------------------------
    // Create application
    //---------------------------------

    Application app;



    //---------------------------------
    // Start server
    //---------------------------------

    app.start();



    Server& server =
        app.getServer();



    ConnectionManager& manager =
        server.getConnectionManager();



    //---------------------------------
    // Create clients
    //---------------------------------

    int whiteId =
        manager.addConnection();


    int blackId =
        manager.addConnection();


    int observerId =
        manager.addConnection();



    ClientConnection* white =
        manager.getConnection(
            whiteId);


    ClientConnection* black =
        manager.getConnection(
            blackId);


    ClientConnection* observer =
        manager.getConnection(
            observerId);



    if(!white ||
       !black ||
       !observer)
    {
        std::cout
            << "Client creation failed\n";

        return 1;
    }



    std::cout
        << "Clients created\n\n";



    //---------------------------------
    // Test incoming command path
    //---------------------------------

    std::cout
        << "Command Test\n"
        << "-------------\n";



    NetworkMessage select(
        MessageType::COMMAND,
        "CLICK 6 0");



    MoveResult result =
        white->send(
            select);



    std::cout
        << "White select : "
        << result.reason
        << std::endl;



    NetworkMessage move(
        MessageType::COMMAND,
        "CLICK 5 0");



    result =
        white->send(
            move);



    std::cout
        << "White move   : "
        << result.reason
        << std::endl;



    NetworkMessage invalid(
        MessageType::COMMAND,
        "HELLO");



    result =
        black->send(
            invalid);



    std::cout
        << "Black invalid: "
        << result.reason
        << std::endl;




    //---------------------------------
    // Test outgoing message path
    //---------------------------------

    std::cout
        << "\nBroadcast Test\n"
        << "--------------\n";



    NetworkMessage testBroadcast(
        MessageType::GAME_STATE,
        "{ \"test\" : \"snapshot\" }");



    manager.broadcast(
        testBroadcast);



    if(white->getLastMessage())
    {
        std::cout
            << "White received message\n";
    }


    if(black->getLastMessage())
    {
        std::cout
            << "Black received message\n";
    }


    if(observer->getLastMessage())
    {
        std::cout
            << "Observer received message\n";
    }




    //---------------------------------
    // Remove observer
    //---------------------------------

    manager.removeConnection(
        observerId);



    std::cout
        << "\nObserver removed\n";



    std::cout
        << "Active clients: "
        << manager.size()
        << std::endl;




    //---------------------------------
    // Stop
    //---------------------------------

    app.stop();



    std::cout
        << "\n============================\n"
        << " Test Finished\n"
        << "============================\n";



    return 0;
}