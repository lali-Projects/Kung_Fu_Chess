#include "GameLoop.hpp"


GameLoop::GameLoop(
    GameEngine& engine,
    GameSnapshotBuilder& snapshotBuilder,
    GameRenderer& renderer,
    Window& window,
    int width,
    int height,
    int fps)
    :
    gameEngine(engine),
    snapshotBuilder(snapshotBuilder),
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

    const int frameTime = 1000 / fps;

    auto previousFrame =
        std::chrono::steady_clock::now();

    while(running && window.isOpen())
    {
        auto currentFrame =
            std::chrono::steady_clock::now();

        int deltaTime =
            static_cast<int>(
                std::chrono::duration_cast<
                    std::chrono::milliseconds>(
                        currentFrame - previousFrame)
                .count());

        previousFrame = currentFrame;

        //---------------------------------
        // Advance game time
        //---------------------------------

        gameEngine.wait(deltaTime);

        //---------------------------------
        // Build snapshot
        //---------------------------------

        GameSnapshot snapshot =
            snapshotBuilder.build();

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

        window.show(canvas);

        //---------------------------------
        // FPS limit
        //---------------------------------

        auto frameEnd =
            std::chrono::steady_clock::now();

        int elapsed =
            static_cast<int>(
                std::chrono::duration_cast<
                    std::chrono::milliseconds>(
                        frameEnd - currentFrame)
                .count());

        if(elapsed < frameTime)
        {
            cv::waitKey(frameTime - elapsed);
        }
    }
}



void GameLoop::stop()
{
    running = false;
}