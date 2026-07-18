#include "PieceRenderer.hpp"
#include "SpriteKey.hpp"

PieceRenderer::PieceRenderer(const Layout& layout, TextureManager& textureManager)
    : layout(layout), textureManager(textureManager) {}

void PieceRenderer::draw(Img& canvas, const GameSnapshot& snapshot)
{
    for (const auto& piece : snapshot.getPieces()) {
        std::string key = SpriteKey::create(piece);

        if (!textureManager.contains(key)) {
            std::cout << "Texture not found: " << key << std::endl;
            continue;
        }

        Img& sprite = textureManager.getTexture(key);
        cv::Point pixel = layout.boardToPixel(piece.position);

        sprite.draw_on(canvas, pixel.x, pixel.y);
    }
}