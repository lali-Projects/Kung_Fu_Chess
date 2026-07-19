#pragma once

#include "Layout.hpp"
#include "TextureManager.hpp"
#include "GameSnapshot.hpp"
#include "AnimationPlayer.hpp"


class PieceRenderer
{

private:

    const Layout& layout;

    TextureManager& textureManager;

    AnimationPlayer animationPlayer;



private:

    /**
     * מחשב מיקום פיקסלים בזמן תנועה.
     */
    cv::Point calculateRenderPosition(
        const PieceSnapshot& piece,
        int currentTime) const;



public:

    PieceRenderer(
        const Layout& layout,
        TextureManager& textureManager);



    void draw(
        Img& canvas,
        const GameSnapshot& snapshot);

};