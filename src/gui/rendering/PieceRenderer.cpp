#include "PieceRenderer.hpp"

#include "SpriteKey.hpp"



PieceRenderer::PieceRenderer(
    const Layout& layout,
    TextureManager& textureManager)
    :
    layout(layout),
    textureManager(textureManager)
{
}



void PieceRenderer::draw(
    Img& canvas,
    const GameSnapshot& snapshot)
{

    for(const auto& piece :
        snapshot.getPieces())
    {

    std::cout
    << "Piece ID: " << piece.id
    << "  row=" << piece.position.getRow()
    << "  col=" << piece.position.getCol()
    << std::endl;
        std::string key =
            SpriteKey::create(piece);

std::cout << "Key = " << key << std::endl;

        if(!textureManager.contains(key))
        { 
             std::cout << "Texture not found: " << key << std::endl;
            continue;
        }



   Img& sprite =
    textureManager.getTexture(key);

cv::Point pixel =
    layout.boardToPixel(piece.position);

std::cout
    << "Draw at: "
    << pixel.x
    << ", "
    << pixel.y
    << std::endl;
std::cout
    << "Sprite channels: "
    << sprite.get_mat().channels()
    << std::endl;

std::cout
    << "Canvas channels: "
    << canvas.get_mat().channels()
    << std::endl;
sprite.draw_on(
    canvas,
    pixel.x,
    pixel.y);
}}