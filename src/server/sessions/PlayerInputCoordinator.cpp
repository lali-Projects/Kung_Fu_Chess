#include "PlayerInputCoordinator.hpp"

#include "Board.hpp"
#include "ClickCommand.hpp"
#include "GameContext.hpp"
#include "GameController.hpp"
#include "Piece.hpp"
#include "PlayerSession.hpp"

PlayerInputCoordinator::PlayerInputCoordinator(GameContext& context)
    : m_context(context)
{
}

void PlayerInputCoordinator::addPlayer(const PlayerSession& player)
{
    m_selections[player.getSessionId()] = std::nullopt;
}

void PlayerInputCoordinator::removePlayer(const PlayerSession& player)
{
    m_selections.erase(player.getSessionId());
}

MoveResult PlayerInputCoordinator::handleClick(
    PlayerSession& player,
    const ClickCommand& command
)
{
    auto& selection = m_selections[player.getSessionId()];

    const Position& position = command.getPosition();

    if (!selection)
    {
        auto clickedPiece = m_context.getBoard().getPieceAt(position);

        if (clickedPiece && clickedPiece->getSide() != player.getSide())
        {
            return {false, "piece_not_owned_by_player"};
        }
    }
    else
    {
        auto selectedPiece = m_context.getBoard().getPieceAt(*selection);

        if (!selectedPiece || selectedPiece->getSide() != player.getSide())
        {
            selection.reset();
            return {false, "selection_invalid"};
        }
    }

    return m_context.getController().click(position, selection);
}

std::optional<Position> PlayerInputCoordinator::getSelectedPosition(
    const PlayerSession& player
) const
{
    auto iterator = m_selections.find(player.getSessionId());

    if (iterator == m_selections.end())
    {
        return std::nullopt;
    }

    return iterator->second;
}