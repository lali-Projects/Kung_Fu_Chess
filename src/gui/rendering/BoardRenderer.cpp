#include "BoardRenderer.hpp"



BoardRenderer::BoardRenderer(const Layout& layout, TextureManager& textureManager):
    layout(layout),textureManager(textureManager)
{
}



void BoardRenderer::draw(Img& canvas)
{
    Img boardTexture =textureManager.getTexture("board");


    boardTexture.resize(layout.getBoardWidth(), layout.getBoardHeight());

    boardTexture.draw_on( canvas, layout.getBoardOffsetX(), layout.getBoardOffsetY());
}