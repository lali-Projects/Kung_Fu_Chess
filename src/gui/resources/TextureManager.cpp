#include "TextureManager.hpp"

#include <stdexcept>

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