#pragma once

#include "ClientPacket.hpp"
#include "Position.hpp"


/**
 * @brief Packet sent from client when
 * a board cell is clicked.
 *
 * This packet only transfers input data.
 *
 * It does NOT:
 * - Decide move or jump.
 * - Validate rules.
 * - Modify the board.
 */
class ClickPacket : public ClientPacket
{
public:

    /**
     * @brief Creates click packet.
     *
     * @param position Clicked board position.
     */
    explicit ClickPacket(
        const Position& position);



    /**
     * @brief Returns packet type.
     */
    PacketType getType() const override;



    /**
     * @brief Returns clicked position.
     */
    const Position& getPosition() const;



private:

    Position m_position;
};