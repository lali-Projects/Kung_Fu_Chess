#pragma once

#include "PieceTypes.hpp"
#include <cstddef>
#include <functional>


/**
 * @class SpriteKey
 * @brief מפתח ייחודי לזיהוי Sprite.
 *
 * משמש את TextureManager כדי למצוא
 * את האנימציה המתאימה לפי:
 *
 * - צבע הכלי
 * - סוג הכלי
 * - מצב הכלי
 *
 * לדוגמה:
 *
 * WHITE + PAWN + IDLE
 *
 * מייצג את התמונות:
 *
 * resources/pieces/PW/idle/sprites/
 */
class SpriteKey
{
private:

    Side side;
    PieceType type;
    PieceState state;


public:

    /**
     * @brief יצירת מפתח Sprite חדש.
     */
    SpriteKey(
        Side side,
        PieceType type,
        PieceState state
    );


    Side getSide() const;


    PieceType getType() const;


    PieceState getState() const;


    /**
     * @brief השוואה בין שני מפתחות.
     */
    bool operator==(
        const SpriteKey& other
    ) const;
};



/**
 * @brief Hash עבור unordered_map.
 *
 * מאפשר שימוש:
 *
 * unordered_map<SpriteKey, Sprite>
 */
namespace std
{
    template<>
    struct hash<SpriteKey>
    {
        size_t operator()(
            const SpriteKey& key
        ) const
        {
            size_t value = 17;


            value =
                value * 31 +
                static_cast<size_t>(
                    key.getSide()
                );


            value =
                value * 31 +
                static_cast<size_t>(
                    key.getType()
                );


            value =
                value * 31 +
                static_cast<size_t>(
                    key.getState()
                );


            return value;
        }
    };
}