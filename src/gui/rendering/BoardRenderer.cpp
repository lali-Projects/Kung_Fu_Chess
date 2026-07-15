#include "BoardRenderer.hpp"


BoardRenderer::BoardRenderer(
    const Layout& layout,
    TextureManager& textureManager)
    :
    layout(layout),
    textureManager(textureManager)
{
}


void BoardRenderer::draw(Img& canvas)
{
    Img& boardImage =
        textureManager.getTexture("board");


    boardImage.draw_on(
        canvas,
        layout.getBoardOffsetX(),
        layout.getBoardOffsetY());
}