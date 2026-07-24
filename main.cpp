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
#include "PlayerSession.hpp"
#include "Server.hpp"
#include "ConnectionManager.hpp"
#include "ClientConnection.hpp"

#include "GameSnapshot.hpp"



int main()
{

    Application app;


    app.start();



    Server& server =
        app.getServer();



    ConnectionManager& manager =
        server.getConnectionManager();



    //---------------------------------
    // Create clients
    //---------------------------------

    int player1Id =
        manager.addConnection();


    int player2Id =
        manager.addConnection();


    int observerId =
        manager.addConnection();



    ClientConnection* player1 =
        manager.getConnection(
            player1Id);



    ClientConnection* player2 =
        manager.getConnection(
            player2Id);



    ClientConnection* observer =
        manager.getConnection(
            observerId);



    if(!player1 ||
       !player2 ||
       !observer)
    {
        std::cout
            << "Client creation failed"
            << std::endl;

        return 1;
    }



    //---------------------------------
    // Check roles
    //---------------------------------

    std::cout
        << "Player1 side = "
        << static_cast<int>(
            player1->getPlayerSession().getSide())
        << std::endl;



    std::cout
        << "Player2 side = "
        << static_cast<int>(
            player2->getPlayerSession().getSide())
        << std::endl;



    std::cout
        << "Observer side = "
        << static_cast<int>(
            observer->getPlayerSession().getSide())
        << std::endl;



    //---------------------------------
    // Connections
    //---------------------------------

    std::cout
        << "Connections = "
        << manager.size()
        << std::endl;



    //---------------------------------
    // Player 1 move
    //---------------------------------

    MoveResult select =
        player1->send(
            "CLICK 6 0");



    std::cout
        << "Player1 select : "
        << select.reason
        << std::endl;



    MoveResult move =
        player1->send(
            "CLICK 5 0");



    std::cout
        << "Player1 move   : "
        << move.reason
        << std::endl;



    //---------------------------------
    // Check snapshots
    //---------------------------------

    std::cout
        << "\nSnapshot delivery test"
        << std::endl;



    bool p1Snapshot =
        player1->getLastSnapshot()
        .has_value();



    bool p2Snapshot =
        player2->getLastSnapshot()
        .has_value();



    bool observerSnapshot =
        observer->getLastSnapshot()
        .has_value();



    std::cout
        << "Player1 snapshot : "
        << (p1Snapshot ? "received" : "missing")
        << std::endl;



    std::cout
        << "Player2 snapshot : "
        << (p2Snapshot ? "received" : "missing")
        << std::endl;



    std::cout
        << "Observer snapshot : "
        << (observerSnapshot ? "received" : "missing")
        << std::endl;



    //---------------------------------
    // Observer cannot play
    //---------------------------------

    MoveResult observerMove =
        observer->send(
            "CLICK 1 0");



    std::cout
        << "Observer action : "
        << observerMove.reason
        << std::endl;



    //---------------------------------
    // Remove connection
    //---------------------------------

    manager.removeConnection(
        observerId);



    std::cout
        << "Connections after remove = "
        << manager.size()
        << std::endl;



    //---------------------------------
    // Stop
    //---------------------------------

    app.stop();



    return 0;
}