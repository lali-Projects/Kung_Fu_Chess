#include "GameLoop.hpp"



GameLoop::GameLoop(
    GameEngine& engine,
    GameRenderer& renderer,
    Window& window,
    int width,
    int height,
    int fps)
    :
    gameEngine(engine),
    renderer(renderer),
    window(window),
    canvas(width, height),
    running(false),
    fps(fps)
{
}



void GameLoop::run()
{
    running = true;


    const int frameTime =
        1000 / fps;



    while(running && window.isOpen())
    {

        auto start =
            std::chrono::steady_clock::now();



        //---------------------------------
        // Get current game state
        //---------------------------------

        GameSnapshot snapshot =
            gameEngine.getSnapshot();



        //---------------------------------
        // Render
        //---------------------------------
        canvas.clear();

        renderer.render(
            canvas,
            snapshot);



        //---------------------------------
        // Display
        //---------------------------------

        window.show(
            canvas);



        //---------------------------------
        // FPS control
        //---------------------------------

        auto end =
            std::chrono::steady_clock::now();


        auto elapsed =
            std::chrono::duration_cast<
                std::chrono::milliseconds>(
                    end - start)
                .count();



        if(elapsed < frameTime)
        {
            cv::waitKey(
                frameTime - elapsed);
        }


    }
}



void GameLoop::stop()
{
    running = false;
}