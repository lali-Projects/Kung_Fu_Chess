#include "SnapshotSerializer.hpp"

#include <nlohmann/json.hpp>

#include "PieceSnapshot.hpp"

using json = nlohmann::json;

namespace
{
    json positionToJson(const Position& position)
    {
        return {
            {"row", position.row},
            {"col", position.col}
        };
    }
}

//================================================
// Serialize
//================================================

NetworkMessage SnapshotSerializer::serialize(const GameSnapshot& snapshot)
{
    std::string jsonString = toJson(snapshot);

    return NetworkMessage(MessageType::GAME_STATE, jsonString);
}

//================================================
// To JSON
//================================================

std::string SnapshotSerializer::toJson(const GameSnapshot& snapshot)
{
    json root;

    //---------------------------------
    // Game state
    //---------------------------------

    root["current_time"] = snapshot.getCurrentTime();
    root["game_over"] = snapshot.isGameOver();

    //---------------------------------
    // Selected position
    //---------------------------------

    if (snapshot.getSelectedPosition())
    {
        root["selected_position"] = {
            {"row", snapshot.getSelectedPosition()->row},
            {"col", snapshot.getSelectedPosition()->col}
        };
    }
    else
    {
        root["selected_position"] = nullptr;
    }

    //---------------------------------
    // Pieces
    //---------------------------------

    json pieces = json::array();

    for (const auto& piece : snapshot.getPieces())
    {
        json item;

        item["id"] = piece.id;
        item["row"] = piece.position.row;
        item["col"] = piece.position.col;
        item["type"] = static_cast<int>(piece.type);
        item["side"] = static_cast<int>(piece.side);
        item["state"] = static_cast<int>(piece.state);

        item["animation_start_time"] = piece.animationStartTime;
        item["has_active_animation"] = piece.hasActiveAnimation;
        item["has_motion"] = piece.hasMotion;
        item["motion_start"] = positionToJson(piece.motionStart);
        item["motion_destination"] = positionToJson(piece.motionDestination);
        item["motion_start_time"] = piece.motionStartTime;
        item["motion_end_time"] = piece.motionEndTime;

        pieces.push_back(item);
    }

    root["pieces"] = pieces;

    return root.dump();
}