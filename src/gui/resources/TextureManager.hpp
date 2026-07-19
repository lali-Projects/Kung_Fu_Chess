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
 *
 * @brief מנהל כל משאבי התצוגה של המשחק.
 *
 * אחריות:
 * - טעינת תמונות סטטיות.
 * - טעינת אנימציות.
 * - שמירת משאבים בזיכרון.
 *
 * אינו אחראי:
 * - ציור.
 * - בחירת פריים.
 * - ניהול זמן.
 */
class TextureManager
{

private:

    //---------------------------------
    // Static textures
    //---------------------------------

    std::unordered_map<
        std::string,
        Img> textures;



    //---------------------------------
    // Animations
    //---------------------------------

    std::unordered_map<
        std::string,
        Animation> animations;



private:


    /**
     * @brief טוען Sprite מתוך תיקיית sprites.
     *
     * כל PNG בתיקייה הופך ל־Frame.
     */
    Sprite loadSprite(
        const std::string& spritesPath,
        int cellSize);



    /**
     * @brief קורא config.json
     * ויוצר Animation.
     */
    Animation loadAnimation(
        const std::string& animationPath,
        int cellSize);



public:


    TextureManager() = default;



    //---------------------------------
    // Images
    //---------------------------------


    void loadTexture(
        const std::string& key,
        const std::string& path);



    void loadTexture(
        const std::string& key,
        const std::string& path,
        int width,
        int height);



    Img& getTexture(
        const std::string& key);



    bool contains(
        const std::string& key) const;



    void loadBoardTexture(
        const std::string& path);



    //---------------------------------
    // Animations
    //---------------------------------


    /**
     * @brief טוען את כל אנימציות הכלים.
     */
    void loadAllPieceAnimations(
        int cellSize);



    Animation& getAnimation(
        const std::string& key);



    bool containsAnimation(
        const std::string& key) const;

};