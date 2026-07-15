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
 */
class TextureManager
{
private:

    std::unordered_map<std::string, Img> textures;

public:

    TextureManager() = default;

    /**
     * @brief טעינת תמונה בגודלה המקורי.
     */
    void loadTexture(
        const std::string& key,
        const std::string& path);

    /**
     * @brief טעינת תמונה ושינוי גודל בזמן הטעינה.
     */
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
};