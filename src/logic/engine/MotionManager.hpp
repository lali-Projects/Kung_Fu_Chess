#pragma once

#include <optional>
#include <memory>

#include "Motion.hpp"
#include "GameConfig.hpp"

/**
 * @class MotionManager
 * @brief מנהל את מחזור החיים של תנועות רגילות.
 *
 * אחראי רק על ניהול Motion בזמן.
 */
class MotionManager
{
private:
    std::optional<Motion> activeMotion;

public:
    /**
     * @brief מתחיל תנועה חדשה.
     */
    bool startMotion(std::shared_ptr<Piece> piece, Position source, Position destination, int startTime);

    /**
     * @brief בודק האם קיימת תנועה פעילה.
     */
    bool hasActiveMotion() const;

    /**
     * @brief בודק האם התנועה הסתיימה.
     */
    bool hasFinished(int currentTime) const;

    /**
     * @brief מחזיר את התנועה הפעילה.
     */
    const Motion& getActiveMotion() const;

    /**
     * @brief מסיים ומוחק Motion.
     */
    void clear();

    bool hasActiveMotionFor(std::shared_ptr<Piece> piece) const;

Position getMotionStart(std::shared_ptr<Piece> piece) const;

Position getMotionDestination(std::shared_ptr<Piece> piece) const;

int getMotionStartTime(std::shared_ptr<Piece> piece) const;

int getMotionFinishTime(std::shared_ptr<Piece> piece) const;
};