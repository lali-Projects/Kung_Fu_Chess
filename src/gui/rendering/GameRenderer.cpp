#include "GameRenderer.hpp"



GameRenderer::GameRenderer(const Layout& layout, TextureManager& textureManager):
    boardRenderer(layout,textureManager),
    pieceRenderer( layout,textureManager),
    selectionRenderer(layout)
{
}



void GameRenderer::render(Img& canvas, const GameSnapshot& snapshot)
{
    canvas.clear();

    boardRenderer.draw(canvas);

    pieceRenderer.draw(canvas,snapshot);

    selectionRenderer.draw(canvas,snapshot);
 
}