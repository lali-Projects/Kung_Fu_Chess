#pragma once

#include "Img.hpp"
#include "GameSnapshot.hpp"

#include "BoardRenderer.hpp"
#include "PieceRenderer.hpp"


class GameRenderer
{

private:

    BoardRenderer boardRenderer;

    PieceRenderer pieceRenderer;


public:

    GameRenderer(
        const Layout& layout,
        TextureManager& textureManager);



    void render(
        Img& canvas,
        const GameSnapshot& snapshot);

};