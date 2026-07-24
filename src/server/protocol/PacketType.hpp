#pragma once

enum class PacketType
{
    UNKNOWN,

    LOGIN,
    REGISTER,

    PLAY,

    CREATE_ROOM,
    JOIN_ROOM,

    CLICK,

    HEARTBEAT,

    GAME_STATE,

    FULL_SNAPSHOT,
    DELTA_SNAPSHOT,

    ERROR_PACKET
};