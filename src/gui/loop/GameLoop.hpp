#pragma once

#include "GameEngine.hpp"
#include "GameSnapshotBuilder.hpp"
#include "GameRenderer.hpp"
#include "Window.hpp"
#include "Img.hpp"
#include <chrono>

/**
 * @class GameLoop
 * @brief מנהלת את הלולאה הראשית של התוכנה, האחראית על תיאום בין הלוגיקה, הרינדור והתצוגה.
 */
class GameLoop
{
private:
    GameEngine& gameEngine;              
    GameSnapshotBuilder& snapshotBuilder; 
    GameRenderer& renderer;               
    Window& window;                      
    Img canvas;                           
    bool running;                        
    int fps;                              
public:
    /**
     * @brief בנאי לאתחול הלולאה והרכיבים הדרושים לה.
     */
    GameLoop(
        GameEngine& engine,
        GameSnapshotBuilder& snapshotBuilder,
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