#pragma once

#include "Img.hpp"

#include "Layout.hpp"
#include "TextureManager.hpp"
#include "BoardRenderer.hpp"

#include "SelectionRenderer.hpp"
#include "PieceRenderer.hpp"
class GameRenderer
{

private:

    BoardRenderer boardRenderer;

    PieceRenderer pieceRenderer;
   
    SelectionRenderer selectionRenderer;

public:

    GameRenderer(
        const Layout& layout,
        TextureManager& textureManager);



    void render(
        Img& canvas,
        const GameSnapshot& snapshot);

};