#pragma once

#include "Img.hpp"
#include "Layout.hpp"
#include "TextureManager.hpp"


/**
 * @class BoardRenderer
 * @brief אחראי על ציור לוח המשחק בלבד.
 *
 * מכיר:
 * - Layout
 * - TextureManager
 *
 * אינו מכיר:
 * - GameEngine
 * - Board
 * - Piece
 * - חוקי משחק
 */
class BoardRenderer
{
private:

    const Layout& layout;

    TextureManager& textureManager;


public:

    /**
     * @brief בנאי.
     */
    BoardRenderer(const Layout& layout, TextureManager& textureManager);
    
    /**
     * @brief מצייר את הלוח על Canvas.
     */
    void draw(Img& canvas);
};