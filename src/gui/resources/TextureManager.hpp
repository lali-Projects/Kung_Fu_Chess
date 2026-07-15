#pragma once

#include <unordered_map>
#include <string>

#include "Img.hpp"


/**
 * @class TextureManager
 * @brief מנהל טעינה ושמירה של תמונות.
 *
 * אחראי:
 * - טעינה חד פעמית של תמונות.
 * - שמירתן בזיכרון.
 * - החזרת תמונה לפי מפתח.
 *
 * אינו מכיר:
 * - משחק
 * - כלים
 * - לוח
 */
class TextureManager
{
private:

    std::unordered_map<std::string, Img> textures;


public:

    TextureManager() = default;


    /**
     * טעינת תמונה ושמירתה תחת מפתח.
     */
    void loadTexture(
        const std::string& key,
        const std::string& path);



    /**
     * קבלת תמונה לפי מפתח.
     */
    Img& getTexture(
        const std::string& key);



    /**
     * בדיקה האם קיימת תמונה.
     */
    bool contains(
        const std::string& key) const;



    /**
     * טעינת תמונת הלוח.
     */
    void loadBoardTexture(
        const std::string& path);
};