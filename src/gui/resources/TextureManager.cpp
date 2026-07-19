#include "TextureManager.hpp"
#include "GuiConfig.hpp"
#include <algorithm>
#include <stdexcept>

void TextureManager::loadTexture(const std::string& key, const std::string& path)
{
    Img image;
    image.read(path);
    textures[key] = std::move(image);
}

void TextureManager::loadTexture(const std::string& key, const std::string& path, int width, int height)
{
    Img image;
    image.read(path, {width, height}, false);
    textures[key] = std::move(image);
}

Img& TextureManager::getTexture(const std::string& key)
{
    auto it = textures.find(key);
    if (it == textures.end())
    {
        throw std::runtime_error("Texture not found: " + key);
    }
    return it->second;
}

bool TextureManager::contains(const std::string& key) const
{
    return textures.find(key) != textures.end();
}

void TextureManager::loadBoardTexture(const std::string& path)
{
    loadTexture("board", path);
}

Sprite TextureManager::loadSprite(const std::string& spritesPath, int cellSize)
{
    Sprite sprite;
    std::vector<fs::path> frames;

    for (auto& file : fs::directory_iterator(spritesPath))
    {
        if (file.path().extension() == ".png")
        {
            frames.push_back(file.path());
        }
    }

    std::sort(frames.begin(), frames.end());

    for (auto& framePath : frames)
    {
        Img frame;
        frame.read(framePath.string(), {cellSize, cellSize}, false);
        sprite.addFrame(frame);
    }

    return sprite;
}

Animation TextureManager::loadAnimation(const std::string& animationPath, int cellSize)
{
    std::string jsonPath = animationPath + "/config.json";
    std::string spritesPath = animationPath + "/sprites";

    Sprite sprite = loadSprite(spritesPath, cellSize);

    std::ifstream file(jsonPath);
    if (!file)
    {
        throw std::runtime_error("Cannot open " + jsonPath);
    }

    json data;
    file >> data;

    Animation animation;
    animation.setSprite(sprite);
    animation.setFramesPerSecond(data["graphics"]["frames_per_sec"]);
    animation.setLoop(data["graphics"]["is_loop"]);
    animation.setSpeed(data["physics"]["speed_m_per_sec"]);
    animation.setNextState(data["physics"]["next_state_when_finished"]);

    return animation;
}

void TextureManager::loadAllPieceAnimations(int cellSize)
{
    std::vector<std::string> pieces = {
        "WK", "WQ", "WR", "WB", "WN", "WP",
        "BK", "BQ", "BR", "BB", "BN", "BP"
    };

    std::vector<std::string> states = {
        "idle", "move", "jump", "long_rest", "short_rest"
    };

    for (auto& piece : pieces)
    {
        for (auto& state : states)
        {
            std::string path = GuiConfig::PIECES_PATH + "/" + piece + "/states/" + state;
            animations[piece + "_" + state] = loadAnimation(path, cellSize);
        }
    }
}

Animation& TextureManager::getAnimation(const std::string& key)
{
    auto it = animations.find(key);
    if (it == animations.end())
    {
        throw std::runtime_error("Animation not found: " + key);
    }
    return it->second;
}

bool TextureManager::containsAnimation(const std::string& key) const
{
    return animations.find(key) != animations.end();
}