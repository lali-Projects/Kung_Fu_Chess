#pragma once

#include <unordered_map>
#include <string>

#include "Sprite.hpp"
#include "SpriteKey.hpp"


/**
 * @class TextureManager
 * @brief מנהל את כל התמונות של המשחק.
 *
 * אחראי על טעינה ושמירה של Sprites.
 */
class TextureManager
{

private:

    std::unordered_map<SpriteKey, Sprite> textures;

    std::string basePath;


    /**
     * @brief טוען Sprite מתיקייה מסוימת.
     */
    Sprite loadSpriteFromFolder(
        const std::string& folderPath
    );


    /**
     * @brief בניית שם תיקיית הכלי.
     */
    std::string buildPieceFolder(
        Side side,
        PieceType type
    ) const;


    /**
     * @brief בניית שם תיקיית המצב.
     */
    std::string buildStateFolder(
        PieceState state
    ) const;


public:


    /**
     * @brief בנאי.
     *
     * @param path נתיב לתיקיית pieces.
     */
    explicit TextureManager(
        const std::string& path
    );


    /**
     * @brief טעינת כל התמונות.
     */
    void loadAll();


    /**
     * @brief החזרת Sprite לפי מפתח.
     */
    const Sprite& getSprite(
        const SpriteKey& key
    ) const;


    /**
     * @brief בדיקה האם קיים Sprite.
     */
    bool contains(
        const SpriteKey& key
    ) const;


    /**
     * @brief מחיקת כל הטקסטורות.
     */
    void clear();

};