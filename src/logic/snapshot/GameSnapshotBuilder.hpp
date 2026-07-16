#pragma once

#include "GameEngine.hpp"
#include "GameController.hpp"
#include "GameSnapshot.hpp"


/**
 * @class GameSnapshotBuilder
 * @brief בונה Snapshot מלא עבור שכבת התצוגה.
 *
 * מאחד:
 * - מצב המשחק שמגיע מ-GameEngine
 * - מצב ממשק המשתמש שמגיע מ-GameController
 *
 * המחלקה אינה משנה את המשחק.
 * היא רק יוצרת אובייקט מידע עבור ה-GUI.
 */
class GameSnapshotBuilder
{
private:

    const GameEngine& gameEngine;

    const GameController& controller;


public:

    GameSnapshotBuilder(
        const GameEngine& engine,
        const GameController& controller);


    GameSnapshot build() const;
};