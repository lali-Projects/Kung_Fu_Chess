
#include "GameLoop.hpp"
#include "GuiConfig.hpp"

GameLoop::GameLoop(IGameFrameSource& frameSource, GameRenderer& renderer, Window& window, int width, int height, int fps)
    : frameSource(frameSource), renderer(renderer), window(window), canvas(width, height), running(false), stopRequested(false), fps(fps)
{
}

void GameLoop::run()
{
    if(stopRequested.load())
    {
        return;
    }
    running = true;
    if(stopRequested.load())
    {
        running = false;
        return;
    }
    const int frameTime = GuiConfig::MS_PER_SECOND / fps;
    auto previousFrame = std::chrono::steady_clock::now();
    
    while (running && window.isOpen())
    {
        auto currentFrame = std::chrono::steady_clock::now();
        int deltaTime = static_cast<int>(std::chrono::duration_cast<std::chrono::milliseconds>(currentFrame - previousFrame).count());
        previousFrame = currentFrame;

        const auto snapshot = frameSource.getFrame(deltaTime);
        if(snapshot)
        {
            renderer.render(canvas, *snapshot);
        }

        window.show(canvas);

        auto frameEnd = std::chrono::steady_clock::now();
        int elapsed = static_cast<int>(std::chrono::duration_cast<std::chrono::milliseconds>(frameEnd - currentFrame).count());

        if (elapsed < frameTime)
        {
            cv::waitKey(frameTime - elapsed);
        }
    }
    running.store(false);
}

void GameLoop::stop()
{
    stopRequested.store(true);
    running.store(false);
}
