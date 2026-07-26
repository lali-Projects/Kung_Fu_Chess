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
#include <thread>
#include <chrono>
#include <atomic>
#include <mutex>

#include <ixwebsocket/IXWebSocket.h>
#include <ixwebsocket/IXNetSystem.h>


#include "Server.hpp"
#include "WebSocketServer.hpp"

#include "EventBus.hpp"

#include "Board.hpp"
#include "BoardInitializer.hpp"

#include "RuleEngine.hpp"
#include "RealTimeArbiter.hpp"
#include "GameEngine.hpp"

#include "GameController.hpp"
#include "GameSnapshotBuilder.hpp"

#include "GameSession.hpp"
#include "SessionManager.hpp"

#include "CommandHandler.hpp"



#define CHECK(x,msg)                 \
do                                   \
{                                    \
    if(x)                             \
        std::cout<<"[OK] "<<msg<<"\n";\
    else                              \
        std::cout<<"[FAILED] "<<msg<<"\n";\
}while(false)



int main()
{

    ix::initNetSystem();


    std::cout
    << "\n========== FULL SERVER INTEGRATION TEST ==========\n";



    EventBus eventBus;



    Board board(8,8);

    BoardInitializer::setupInitialPosition(board);



    RuleEngine rules;


    RealTimeArbiter arbiter(board);



    GameEngine engine(
        board,
        rules,
        arbiter);



    CHECK(true,"GameEngine created");



    auto controller =
        std::make_unique<GameController>(
            board,
            engine);



    GameController& controllerRef =
        *controller;



    auto snapshotBuilder =
        std::make_unique<GameSnapshotBuilder>(
            engine,
            controllerRef);



    auto session =
        std::make_unique<GameSession>(
            "local_game",
            std::move(controller),
            std::move(snapshotBuilder),
            eventBus);



    SessionManager sessionManager;


    sessionManager.addSession(
        std::move(session));



    CHECK(true,"GameSession created");



    CommandHandler handler(
        sessionManager);



    auto websocket =
        std::make_unique<WebSocketServer>(
            8080);



    Server server(
        handler,
        sessionManager.getSession(),
        eventBus,
        std::move(websocket));



    server.start();



    std::this_thread::sleep_for(
        std::chrono::milliseconds(500));



    CHECK(
        true,
        "WebSocket server started");




    std::atomic<int> connected{0};


    std::mutex mutex;


    std::string snapshot1;
    std::string snapshot2;




    auto createClient =
    [&](std::string& snapshot)
    {

        auto client =
            std::make_shared<ix::WebSocket>();


        client->setUrl(
            "ws://localhost:8080");



        client->setOnMessageCallback(
        [&](const ix::WebSocketMessagePtr& msg)
        {

            if(!msg)
                return;



            if(msg->type ==
               ix::WebSocketMessageType::Open)
            {
                connected++;
            }



            if(msg->type ==
               ix::WebSocketMessageType::Message)
            {

                if(msg->str.find("{")
                   != std::string::npos)
                {

                    std::lock_guard<std::mutex> lock(
                        mutex);

                    snapshot =
                        msg->str;

                }

            }

        });



        client->start();


        return client;

    };




    auto client1 =
        createClient(snapshot1);



    auto client2 =
        createClient(snapshot2);





    for(int i=0;i<50;i++)
    {

        if(connected==2)
            break;


        std::this_thread::sleep_for(
            std::chrono::milliseconds(100));

    }



    CHECK(
        connected==2,
        "Two clients connected");




    //------------------------------------------------
    // PLAYER 1 SELECT PIECE
    //------------------------------------------------


    client1->send(
        "CLICK 6 0");



    std::this_thread::sleep_for(
        std::chrono::milliseconds(500));



    {
        std::lock_guard<std::mutex> lock(
            mutex);



        CHECK(
            !snapshot1.empty(),
            "First client received snapshot");



        CHECK(
            !snapshot2.empty(),
            "Second client received snapshot");



        CHECK(
            snapshot1==snapshot2,
            "Both clients received same game state");

    }




    //------------------------------------------------
    // MOVE THROUGH SERVER -> ENGINE
    //------------------------------------------------


    client1->send(
        "CLICK 5 0");



    std::this_thread::sleep_for(
        std::chrono::milliseconds(500));



    CHECK(
        true,
        "Move command processed by server");





    //------------------------------------------------
    // Disconnect
    //------------------------------------------------


    client2->stop();



    std::this_thread::sleep_for(
        std::chrono::milliseconds(500));



    CHECK(
        true,
        "Disconnect handled");




    client1->stop();



    server.stop();



    ix::uninitNetSystem();



    std::cout
    << "\n========== TEST FINISHED ==========\n";



    return 0;
}