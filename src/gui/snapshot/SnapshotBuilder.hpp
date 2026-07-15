#pragma once

#include "GameSnapshot.hpp"

// Forward Declaration
class GameEngine;

/**
 * @class SnapshotBuilder
 * @brief בונה GameSnapshot מתוך מצב המשחק.
 *
 * זו המחלקה היחידה בשכבת ה-GUI
 * שמכירה את שכבת הלוגיקה.
 */
class SnapshotBuilder
{
public:

    /**
     * @brief בונה צילום מצב מלא של המשחק.
     */
    static GameSnapshot build(
        const GameEngine& engine
    );
};