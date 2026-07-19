#pragma once

#include "Layout.hpp"
#include "TextureManager.hpp"
#include "GameSnapshot.hpp"
#include "AnimationPlayer.hpp"

/**
 * @class PieceRenderer
 * @brief Responsible for handling the visual representation of game pieces.
 * 
 * This class translates logical piece snapshots into graphical elements
 * displayed on the canvas, utilizing the layout configuration and texture management.
 */
class PieceRenderer
{
private:
    const Layout& layout;
    TextureManager& textureManager;
    AnimationPlayer animationPlayer;

    /**
     * @brief Calculates the exact pixel position for a piece, accounting for animation states.
     * 
     * @param piece The snapshot of the piece to render.
     * @param currentTime The current game time for animation calculation.
     * @return cv::Point The calculated pixel coordinate.
     */
    cv::Point calculateRenderPosition(const PieceSnapshot& piece, int currentTime) const;

public:
    /**
     * @brief Constructs the PieceRenderer with necessary dependencies.
     */
    PieceRenderer(const Layout& layout, TextureManager& textureManager);

    /**
     * @brief Draws the pieces from the provided snapshot onto the canvas.
     * 
     * @param canvas The image surface to draw on.
     * @param snapshot The current state of the board.
     */
    void draw(Img& canvas, const GameSnapshot& snapshot);
};