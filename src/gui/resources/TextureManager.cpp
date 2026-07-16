#include "TextureManager.hpp"

#include <stdexcept>
#include <vector>
#include "GuiConfig.hpp"

void TextureManager::loadTexture(
    const std::string& key,
    const std::string& path)
{
    Img image;

    image.read(path);

    textures[key] = std::move(image);
}

void TextureManager::loadTexture(
    const std::string& key,
    const std::string& path,
    int width,
    int height)
{
    Img image;

    image.read(
        path,
        { width, height },
        false);

    textures[key] = std::move(image);
}

Img& TextureManager::getTexture(
    const std::string& key)
{
    auto iterator = textures.find(key);

    if(iterator == textures.end())
    {
        throw std::runtime_error(
            "Texture not found: " + key);
    }

    return iterator->second;
}

bool TextureManager::contains(
    const std::string& key) const
{
    return textures.find(key) != textures.end();
}

void TextureManager::loadBoardTexture(
    const std::string& path)
{
    loadTexture(
        "board",
        path);
}

void TextureManager::loadAllPieceTextures(int cellSize)
{
    const std::vector<std::string> pieces =
    {
        "WK","WQ","WR","WB","WN","WP",
        "BK","BQ","BR","BB","BN","BP"
    };

    const std::vector<std::string> states =
    {
        "idle",
        "move",
        "jump"
    };

    for (const auto& piece : pieces)
    {
        for (const auto& state : states)
        {
            loadTexture(
                piece + "_" + state,
                GuiConfig::PIECES_PATH + "/" +
                piece +
                "/states/" +
                state +
                "/sprites/1.png",
                cellSize,
                cellSize);
        }
    }
}