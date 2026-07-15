#include "GameRenderer.hpp"



GameRenderer::GameRenderer(
    const Layout& layout,
    TextureManager& textureManager)
    :
    boardRenderer(
        layout,
        textureManager),

    pieceRenderer(
        layout,
        textureManager)
{
}



void GameRenderer::render(
    Img& canvas,
    const GameSnapshot& snapshot)
{

    // ניקוי הקנבס לפני ציור חדש

    canvas.clear();



    // ציור הלוח

    boardRenderer.draw(
        canvas);



    // ציור הכלים

    pieceRenderer.draw(
        canvas,
        snapshot);
}