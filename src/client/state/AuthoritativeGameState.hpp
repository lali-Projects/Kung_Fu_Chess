#pragma once

#include <mutex>
#include <optional>

#include "GameSnapshot.hpp"
#include "Position.hpp"


/** Thread-safe holder for the latest server-authored GUI snapshot. */
class AuthoritativeGameState
{
public:
    void update(GameSnapshot snapshot);
    std::optional<GameSnapshot> getSnapshot() const;
    bool hasSnapshot() const;

    /** Applies per-client presentation selection returned by the server. */
    void setSelectedPosition(
        const std::optional<Position>& position);

    void clear();

private:
    mutable std::mutex m_mutex;
    std::optional<GameSnapshot> m_snapshot;
    std::optional<Position> m_selectedPosition;
    bool m_hasSelectionResponse{false};
};
