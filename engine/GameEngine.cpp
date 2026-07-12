#include "GameEngine.hpp"
/**
 * @brief מבקש להתחיל קפיצה עבור הכלי שנמצא במיקום הנתון.
 *
 * אחריות הפונקציה:
 * 1. לוודא שהמשחק לא הסתיים.
 * 2. לוודא שקיים כלי במיקום.
 * 3. לוודא שהכלי אינו בתנועה.
 * 4. לוודא שהכלי אינו כבר באוויר.
 * 5. לוודא שהכלי לא נלכד.
 * 6. להעביר את הבקשה ל־RealTimeArbiter.
 *
 * הפונקציה אינה מבצעת את הקפיצה בעצמה.
 */
MoveResult GameEngine::requestJump(const Position& position)
{
    // אין פעולות לאחר סיום המשחק
    if (isGameOver)
    {
        return { false, "game_over" };
    }

    // מציאת הכלי המבוקש
    std::shared_ptr<Piece> selectedPiece = board.getPieceAt(position);

    if (selectedPiece == nullptr)
    {
        return { false, "empty_source" };
    }

    // כלי שכבר נע אינו יכול לקפוץ
    if (selectedPiece->getState() == PieceState::MOVING)
    {
        return { false, "piece_is_moving" };
    }

    // כלי שכבר באוויר אינו יכול להתחיל קפיצה נוספת
    if (selectedPiece->getState() == PieceState::AIRBORNE)
    {
        return { false, "piece_is_airborne" };
    }

    // כלי שנלכד אינו יכול לבצע שום פעולה
    if (selectedPiece->getState() == PieceState::CAPTURED)
    {
        return { false, "piece_is_captured" };
    }

    // התחלת הקפיצה מתבצעת ע"י RealTimeArbiter
    realTimeArbiter.startJump(
        selectedPiece,
        currentTimeMs,
        GameConfig::DEFAULT_TRAVEL_TIME_MS
    );

    return { true, "ok" };
}