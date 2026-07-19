#pragma once

#include <unordered_map>
#include <string>
#include <vector>
#include <filesystem>
#include <fstream>

#include <nlohmann/json.hpp>

#include "Img.hpp"
#include "Animation.hpp"
#include "Sprite.hpp"

using json = nlohmann::json;
namespace fs = std::filesystem;

/**
 * @class TextureManager
 * @brief Manages all game visual resources.
 *
 * Responsibilities:
 * - Loading static images.
 * - Loading animations.
 * - Storing resources in memory.
 *
 * Not responsible for:
 * - Rendering.
 * - Frame selection.
 * - Time management.
 */
class TextureManager
{
private:
    //---------------------------------
    // Static textures
    //---------------------------------
    std::unordered_map<std::string, Img> textures;

    //---------------------------------
    // Animations
    //---------------------------------
    std::unordered_map<std::string, Animation> animations;

    /**
     * @brief Loads a Sprite from the sprites directory.
     * Each PNG in the directory becomes a frame.
     */
    Sprite loadSprite(const std::string& spritesPath, int cellSize);

    /**
     * @brief Reads a config.json and creates an Animation object.
     */
    Animation loadAnimation(const std::string& animationPath, int cellSize);

public:
    TextureManager() = default;

    //---------------------------------
    // Images
    //---------------------------------

    /**
     * @brief Loads a texture from the specified path and stores it with a key.
     * @param key The unique identifier for the texture.
     * @param path The file path to the image.
     */
    void loadTexture(const std::string& key, const std::string& path);

    /**
     * @brief Loads a texture from the specified path, resized to given dimensions.
     * @param key The unique identifier for the texture.
     * @param path The file path to the image.
     * @param width The target width.
     * @param height The target height.
     */
    void loadTexture(
        const std::string& key,
        const std::string& path,
        int width,
        int height);

    /**
     * @brief Retrieves a reference to a loaded texture.
     * @param key The unique identifier for the texture.
     * @return A reference to the image object.
     */
    Img& getTexture(const std::string& key);

    /**
     * @brief Checks if a texture exists.
     * @param key The unique identifier for the texture.
     * @return True if the texture exists, false otherwise.
     */
    bool contains(const std::string& key) const;

    /**
     * @brief Loads the texture specifically for the board.
     * @param path The file path to the board image.
     */
    void loadBoardTexture(const std::string& path);

    //---------------------------------
    // Animations
    //---------------------------------

    /**
     * @brief Loads all piece animations.
     * @param cellSize The cell size used for parsing animation frames.
     */
    void loadAllPieceAnimations(int cellSize);

    /**
     * @brief Retrieves a reference to a loaded animation.
     * @param key The unique identifier for the animation.
     * @return A reference to the animation object.
     */
    Animation& getAnimation(const std::string& key);

    /**
     * @brief Checks if an animation exists.
     * @param key The unique identifier for the animation.
     * @return True if the animation exists, false otherwise.
     */
    bool containsAnimation(const std::string& key) const;
};