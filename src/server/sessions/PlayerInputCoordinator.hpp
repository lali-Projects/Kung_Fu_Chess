#pragma once

#include <optional>
#include <string>
#include <unordered_map>

#include "MoveResult.hpp"
#include "Position.hpp"

class ClickCommand;
class GameContext;
class PlayerSession;

/**
 * Owns per-player click selection state at the multiplayer boundary and
 * delegates accepted input to the existing GameController.
 * GameSession serializes calls to this object.
 */
class PlayerInputCoordinator
{
public:
    explicit PlayerInputCoordinator(GameContext& context);

    void addPlayer(const PlayerSession& player);

    void removePlayer(const PlayerSession& player);

    MoveResult handleClick(
        PlayerSession& player,
        const ClickCommand& command
    );

    std::optional<Position> getSelectedPosition(
        const PlayerSession& player
    ) const;

private:
    GameContext& m_context;

    std::unordered_map<std::string, std::optional<Position>> m_selections;
};