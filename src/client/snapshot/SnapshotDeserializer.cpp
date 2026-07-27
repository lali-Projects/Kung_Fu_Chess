#include "SnapshotDeserializer.hpp"

#include <nlohmann/json.hpp>

#include <vector>

#include "PieceSnapshot.hpp"
#include "PieceTypes.hpp"
#include "Position.hpp"


namespace
{
    using Json = nlohmann::json;


    bool validBoardCoordinate(
        int value)
    {
        return
            value >= 0 &&
            value < 8;
    }


    Position parsePosition(
        const Json& value)
    {
        if(!value.is_object() ||
           !value.contains("row") ||
           !value.contains("col") ||
           !value.at("row").is_number_integer() ||
           !value.at("col").is_number_integer())
        {
            throw Json::type_error::create(
                302,
                "position must contain integer row and col",
                &value);
        }

        const int row =
            value.at("row").get<int>();

        const int col =
            value.at("col").get<int>();

        if(!validBoardCoordinate(row) ||
           !validBoardCoordinate(col))
        {
            throw Json::out_of_range::create(
                401,
                "position is outside the 8x8 board",
                &value);
        }

        return Position(row, col);
    }


    bool validSide(
        int value)
    {
        return
            value >= static_cast<int>(Side::NONE) &&
            value <= static_cast<int>(Side::OBSERVER);
    }


    bool validPieceType(
        int value)
    {
        return
            value >= static_cast<int>(PieceType::KING) &&
            value <= static_cast<int>(PieceType::PAWN);
    }


    bool validPieceState(
        int value)
    {
        return
            value >= static_cast<int>(PieceState::IDLE) &&
            value <= static_cast<int>(PieceState::LONG_REST);
    }


    PieceSnapshot parsePiece(
        const Json& item)
    {
        if(!item.is_object())
        {
            throw Json::type_error::create(
                302,
                "piece must be an object",
                &item);
        }

        const int side =
            item.at("side").get<int>();

        const int type =
            item.at("type").get<int>();

        const int state =
            item.at("state").get<int>();

        const int animationStartTime =
            item.at("animation_start_time").get<int>();

        const bool hasActiveAnimation =
            item.at("has_active_animation").get<bool>();

        const bool hasMotion =
            item.at("has_motion").get<bool>();

        const int motionStartTime =
            item.at("motion_start_time").get<int>();

        const int motionEndTime =
            item.at("motion_end_time").get<int>();

        if(!validSide(side) ||
           !validPieceType(type) ||
           !validPieceState(state))
        {
            throw Json::other_error::create(
                501,
                "piece contains an invalid enum value",
                &item);
        }

        if(animationStartTime < 0 ||
           motionStartTime < 0 ||
           motionEndTime < 0 ||
           (hasMotion &&
            motionEndTime < motionStartTime))
        {
            throw Json::out_of_range::create(
                401,
                "piece contains invalid timing data",
                &item);
        }

        const Position position =
            parsePosition(
                {
                    {"row", item.at("row")},
                    {"col", item.at("col")}
                });

        return
        {
            item.at("id").get<int>(),
            static_cast<Side>(side),
            static_cast<PieceType>(type),
            position,
            static_cast<PieceState>(state),
            animationStartTime,
            hasActiveAnimation,
            hasMotion,
            parsePosition(
                item.at("motion_start")),
            parsePosition(
                item.at("motion_destination")),
            motionStartTime,
            motionEndTime
        };
    }
}


std::optional<GameSnapshot>
SnapshotDeserializer::deserialize(
    const NetworkMessage& message)
{
    if(message.getType() !=
       MessageType::GAME_STATE)
    {
        return std::nullopt;
    }

    return deserializePayload(
        message.getPayload());
}


std::optional<GameSnapshot>
SnapshotDeserializer::deserializePayload(
    const std::string& payload)
{
    try
    {
        const Json root =
            Json::parse(payload);

        if(!root.is_object() ||
           !root.contains("pieces") ||
           !root.at("pieces").is_array())
        {
            return std::nullopt;
        }

        const int currentTime =
            root.at("current_time").get<int>();

        if(currentTime < 0)
        {
            return std::nullopt;
        }

        std::vector<PieceSnapshot> pieces;

        pieces.reserve(
            root.at("pieces").size());

        for(const auto& item :
            root.at("pieces"))
        {
            pieces.push_back(
                parsePiece(item));
        }

        std::optional<Position>
            selectedPosition;

        const Json& selected =
            root.at("selected_position");

        if(!selected.is_null())
        {
            selectedPosition =
                parsePosition(selected);
        }

        return GameSnapshot(
            pieces,
            currentTime,
            root.at("game_over").get<bool>(),
            selectedPosition);
    }
    catch(const std::exception&)
    {
        return std::nullopt;
    }
}
