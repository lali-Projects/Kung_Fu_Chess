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
#include <memory>


#include "EventBus.hpp"

#include "RoomFactory.hpp"
#include "RoomManager.hpp"
#include "Room.hpp"

#include "CommandHandler.hpp"
#include "ConnectionManager.hpp"
#include "ClientConnection.hpp"
#include "GameSession.hpp"
#include "PlayerSession.hpp"

#include "NetworkMessage.hpp"

#include "GameStateChangedEvent.hpp"
#include "GameSnapshotBuilder.hpp"




//================================================
// Event Listener
//================================================

class TestListener
{

public:

    void onGameStateChanged(
        const GameStateChangedEvent& event)
    {
        std::cout
            << "[EVENT] Game state changed: "
            << event.getRoomId()
            << std::endl;
    }

};





int main()
{

    std::cout
        << "========== INTEGRATION TEST START =========="
        << std::endl;



    //--------------------------------------------
    // EventBus
    //--------------------------------------------

    EventBus eventBus;



    TestListener listener;



    eventBus.subscribe<GameStateChangedEvent>(
        [&listener]
        (
            std::shared_ptr<GameStateChangedEvent> event
        )
        {
            if(event)
            {
                listener.onGameStateChanged(
                    *event);
            }
        });





    //--------------------------------------------
    // Room system
    //--------------------------------------------

    RoomFactory roomFactory(
        eventBus);



    RoomManager roomManager(
        roomFactory);





    //--------------------------------------------
    // Command system
    //--------------------------------------------

    CommandHandler commandHandler(
        roomManager);





    //--------------------------------------------
    // Connections
    //--------------------------------------------

    ConnectionManager connectionManager(
        commandHandler);



    int whiteConnectionId =
        connectionManager.addConnection();



    int blackConnectionId =
        connectionManager.addConnection();




    ClientConnection* whiteConnection =
        connectionManager.getConnection(
            whiteConnectionId);



    ClientConnection* blackConnection =
        connectionManager.getConnection(
            blackConnectionId);




    if(!whiteConnection ||
       !blackConnection)
    {
        std::cout
            << "FAILED: connections"
            << std::endl;

        return 1;
    }






    //--------------------------------------------
    // Create Room
    //--------------------------------------------

    const std::string roomId =
        "room_1";



    if(!roomManager.createRoom(roomId))
    {
        std::cout
            << "FAILED: create room"
            << std::endl;

        return 1;
    }



    Room* room =
        roomManager.getRoom(roomId);



    if(!room)
    {
        std::cout
            << "FAILED: room missing"
            << std::endl;

        return 1;
    }



    std::cout
        << "[OK] Room created"
        << std::endl;






    //--------------------------------------------
    // Add players
    //--------------------------------------------


    auto whitePlayer =
        whiteConnection->getPlayer();



    auto blackPlayer =
        blackConnection->getPlayer();



    if(!room->getSession()
            .addPlayer(
                whitePlayer))
    {
        std::cout
            << "FAILED: add white"
            << std::endl;

        return 1;
    }




    if(!room->getSession()
            .addPlayer(
                blackPlayer))
    {
        std::cout
            << "FAILED: add black"
            << std::endl;

        return 1;
    }





    std::cout
        << "[OK] Players added"
        << std::endl;







    //--------------------------------------------
    // Send click command
    //--------------------------------------------


    NetworkMessage selectMessage(
        MessageType::COMMAND,
        "CLICK 6 0");



    MoveResult result =
        whiteConnection
            ->receiveNetworkMessage(
                selectMessage);




    std::cout
        << "Selection result: "
        << result.success
        << " "
        << result.reason
        << std::endl;








    //--------------------------------------------
    // Move
    //--------------------------------------------


    NetworkMessage moveMessage(
        MessageType::COMMAND,
        "CLICK 5 0");



    result =
        whiteConnection
            ->receiveNetworkMessage(
                moveMessage);




    std::cout
        << "Move result: "
        << result.success
        << " "
        << result.reason
        << std::endl;








    //--------------------------------------------
    // Snapshot
    //--------------------------------------------


    auto& snapshotBuilder =
        room->getSession()
            .getSnapshotBuilder();



    auto snapshot =
        snapshotBuilder.build();



    std::cout
        << "[OK] Snapshot built"
        << std::endl;






    //--------------------------------------------
    // Broadcast test
    //--------------------------------------------


NetworkMessage stateMessage(
    MessageType::GAME_STATE,
    "STATE_UPDATE");


    connectionManager.broadcast(
        stateMessage);



    std::cout
        << "[OK] Broadcast finished"
        << std::endl;






    std::cout
        << "========== INTEGRATION TEST PASSED =========="
        << std::endl;


    return 0;
}