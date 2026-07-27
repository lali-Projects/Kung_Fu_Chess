#pragma once

#include "GameRenderer.hpp"
#include "IGameFrameSource.hpp"
#include "Window.hpp"
#include "Img.hpp"
#include <atomic>
#include <chrono>

/**
 * @class GameLoop
 * @brief מנהלת את הלולאה הראשית של התוכנה, האחראית על תיאום בין הלוגיקה, הרינדור והתצוגה.
 */
class GameLoop
{
private:
    IGameFrameSource& frameSource;
    GameRenderer& renderer;               
    Window& window;                      
    Img canvas;                           
    std::atomic<bool> running;
    std::atomic<bool> stopRequested;
    int fps;                              
public:
    /**
     * @brief בנאי לאתחול הלולאה והרכיבים הדרושים לה.
     */
    GameLoop(
        IGameFrameSource& frameSource,
        GameRenderer& renderer,
        Window& window,
        int width,
        int height,
        int fps = 60);

    /**
     * @brief מפעילה את הלולאה הראשית המבצעת עדכונים ורינדור בזמן אמת.
     */
    void run();

    /**
     * @brief עוצרת את ריצת הלולאה הראשית.
     */
    void stop();
};
