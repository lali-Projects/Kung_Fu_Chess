#include "PieceRenderer.hpp"

#include "SpriteKey.hpp"

#include <iostream>
#include <algorithm>



PieceRenderer::PieceRenderer(
    const Layout& layout,
    TextureManager& textureManager)
:
layout(layout),
textureManager(textureManager)
{
}

cv::Point PieceRenderer::calculateRenderPosition(
    const PieceSnapshot& piece,
    int currentTime) const
{

    //---------------------------------
    // אין תנועה
    //---------------------------------

    if(!piece.hasMotion)
    {
        return layout.boardToPixel(
            piece.position);
    }



    //---------------------------------
    // זמן תנועה
    //---------------------------------

    int totalTime =
        piece.motionEndTime
        -
        piece.motionStartTime;



    if(totalTime <= 0)
    {
        return layout.boardToPixel(
            piece.motionDestination);
    }



    //---------------------------------
    // כמה מהדרך עברנו
    //---------------------------------

    float progress =
        static_cast<float>(
            currentTime
            -
            piece.motionStartTime)
        /
        static_cast<float>(
            totalTime);



    progress =
        std::clamp(
            progress,
            0.0f,
            1.0f);



    //---------------------------------
    // המרה לפיקסלים
    //---------------------------------

    cv::Point start =
        layout.boardToPixel(
            piece.motionStart);



    cv::Point end =
        layout.boardToPixel(
            piece.motionDestination);



    int x =
        start.x
        +
        static_cast<int>(
            (end.x - start.x)
            *
            progress);



    int y =
        start.y
        +
        static_cast<int>(
            (end.y - start.y)
            *
            progress);



    return cv::Point(x,y);
}






void PieceRenderer::draw(
    Img& canvas,
    const GameSnapshot& snapshot)
{

    for(const auto& piece :
        snapshot.getPieces())
    {


        //---------------------------------
        // Animation key
        //---------------------------------

        std::string key =
            SpriteKey::create(piece);



        //---------------------------------
        // Check animation
        //---------------------------------

        if(!textureManager.containsAnimation(key))
        {
            std::cout
            << "Missing animation: "
            << key
            << std::endl;

            continue;
        }



        const Animation& animation =
            textureManager.getAnimation(key);



        if(animation.empty())
        {
            continue;
        }



        //---------------------------------
        // Current frame
        //---------------------------------

        const Img& frame =
            animationPlayer.getCurrentFrame(
                animation,
                piece.animationStartTime,
                snapshot.getCurrentTime());



        //---------------------------------
        // Position on screen
        //---------------------------------

        cv::Point pixel =
            calculateRenderPosition(
                piece,
                snapshot.getCurrentTime());



        //---------------------------------
        // Draw
        //---------------------------------

        frame.draw_on(
            canvas,
            pixel.x,
            pixel.y);

    }

}