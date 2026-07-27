#include "CommandResponseCodec.hpp"

#include <nlohmann/json.hpp>


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


    Json positionToJson(
        const Position& position)
    {
        return
        {
            {"row", position.row},
            {"col", position.col}
        };
    }


    std::optional<Position> parseNullablePosition(
        const Json& value)
    {
        if(value.is_null())
        {
            return std::nullopt;
        }

        if(!value.is_object() ||
           !value.contains("row") ||
           !value.contains("col") ||
           !value.at("row").is_number_integer() ||
           !value.at("col").is_number_integer())
        {
            throw Json::type_error::create(
                302,
                "selected_position must be null or an integer row/col object",
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
                "selected_position is outside the 8x8 board",
                &value);
        }

        return Position(row, col);
    }


    const char* sideName(
        Side side)
    {
        switch(side)
        {
            case Side::NONE:
                return "NONE";

            case Side::WHITE:
                return "WHITE";

            case Side::BLACK:
                return "BLACK";

            case Side::OBSERVER:
                return "OBSERVER";
        }

        return nullptr;
    }


    std::optional<Side> parseNullableSide(
        const Json& value)
    {
        if(value.is_null())
        {
            return std::nullopt;
        }

        if(!value.is_string())
        {
            throw Json::type_error::create(
                302,
                "side must be null or a string",
                &value);
        }

        const std::string name =
            value.get<std::string>();

        if(name == "NONE")
            return Side::NONE;

        if(name == "WHITE")
            return Side::WHITE;

        if(name == "BLACK")
            return Side::BLACK;

        if(name == "OBSERVER")
            return Side::OBSERVER;

        throw Json::other_error::create(
            501,
            "unknown side value",
            &value);
    }


    void writeOptionalString(
        Json& root,
        const char* key,
        const std::optional<std::string>& value)
    {
        if(value)
        {
            root[key] = *value;
        }
        else
        {
            root[key] = nullptr;
        }
    }


    std::optional<std::string> parseOptionalString(
        const Json& root,
        const char* key)
    {
        if(!root.contains(key) ||
           root.at(key).is_null())
        {
            return std::nullopt;
        }

        if(!root.at(key).is_string())
        {
            throw Json::type_error::create(
                302,
                std::string(key) + " must be null or a string",
                &root.at(key));
        }

        return root.at(key).get<std::string>();
    }
}


std::string CommandResponseCodec::serialize(
    const CommandResponse& response)
{
    Json root;

    root["success"] = response.success;
    root["reason"] = response.reason;
    root["command"] = response.command;

    writeOptionalString(
        root,
        "user_id",
        response.userId);

    writeOptionalString(
        root,
        "username",
        response.username);

    writeOptionalString(
        root,
        "session_id",
        response.sessionId);

    writeOptionalString(
        root,
        "room_id",
        response.roomId);

    if(response.side)
    {
        const char* name =
            sideName(*response.side);

        if(name)
        {
            root["side"] = name;
        }
        else
        {
            root["side"] = nullptr;
        }
    }
    else
    {
        root["side"] = nullptr;
    }

    if(response.selectedPosition)
    {
        root["selected_position"] =
            positionToJson(
                *response.selectedPosition);
    }
    else
    {
        root["selected_position"] = nullptr;
    }

    return root.dump();
}


std::optional<CommandResponse>
CommandResponseCodec::deserialize(
    const std::string& payload)
{
    try
    {
        const Json root =
            Json::parse(payload);

        if(!root.is_object() ||
           !root.contains("success") ||
           !root.at("success").is_boolean() ||
           !root.contains("reason") ||
           !root.at("reason").is_string())
        {
            return std::nullopt;
        }

        CommandResponse response;

        response.success =
            root.at("success").get<bool>();

        response.reason =
            root.at("reason").get<std::string>();

        if(root.contains("command"))
        {
            if(!root.at("command").is_string())
            {
                return std::nullopt;
            }

            response.command =
                root.at("command").get<std::string>();
        }

        response.userId =
            parseOptionalString(
                root,
                "user_id");

        response.username =
            parseOptionalString(
                root,
                "username");

        response.sessionId =
            parseOptionalString(
                root,
                "session_id");

        response.roomId =
            parseOptionalString(
                root,
                "room_id");

        if(root.contains("side"))
        {
            response.side =
                parseNullableSide(
                    root.at("side"));
        }

        if(root.contains("selected_position"))
        {
            response.selectedPosition =
                parseNullablePosition(
                    root.at("selected_position"));
        }

        return response;
    }
    catch(const std::exception&)
    {
        return std::nullopt;
    }
}
