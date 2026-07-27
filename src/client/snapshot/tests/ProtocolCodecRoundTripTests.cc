#include <cassert>
#include <string>
#include <vector>

#include <nlohmann/json.hpp>

#include "CommandResponseCodec.hpp"
#include "GameSnapshot.hpp"
#include "NetworkMessage.hpp"
#include "SnapshotDeserializer.hpp"
#include "SnapshotSerializer.hpp"


namespace
{
    PieceSnapshot makePiece(
        int id,
        Side side,
        PieceType type,
        PieceState state)
    {
        return
        {
            id,
            side,
            type,
            Position(id % 8, (id + 1) % 8),
            state,
            125 + id,
            true,
            true,
            Position(id % 8, (id + 1) % 8),
            Position((id + 2) % 8, (id + 3) % 8),
            100 + id,
            900 + id
        };
    }


    void assertPieceEqual(
        const PieceSnapshot& expected,
        const PieceSnapshot& actual)
    {
        assert(actual.id == expected.id);
        assert(actual.side == expected.side);
        assert(actual.type == expected.type);
        assert(actual.position == expected.position);
        assert(actual.state == expected.state);
        assert(actual.animationStartTime == expected.animationStartTime);
        assert(actual.hasActiveAnimation == expected.hasActiveAnimation);
        assert(actual.hasMotion == expected.hasMotion);
        assert(actual.motionStart == expected.motionStart);
        assert(actual.motionDestination == expected.motionDestination);
        assert(actual.motionStartTime == expected.motionStartTime);
        assert(actual.motionEndTime == expected.motionEndTime);
    }


    void verifySnapshotRoundTrip()
    {
        const std::vector<PieceSnapshot> sourcePieces
        {
            makePiece(0, Side::NONE, PieceType::KING, PieceState::IDLE),
            makePiece(1, Side::WHITE, PieceType::QUEEN, PieceState::MOVING),
            makePiece(2, Side::BLACK, PieceType::ROOK, PieceState::CAPTURED),
            makePiece(3, Side::WHITE, PieceType::BISHOP, PieceState::AIRBORNE),
            makePiece(4, Side::WHITE, PieceType::KNIGHT, PieceState::SHORT_REST),
            makePiece(5, Side::BLACK, PieceType::PAWN, PieceState::LONG_REST)
        };

        const GameSnapshot source(
            sourcePieces,
            450,
            true,
            Position(5, 6));

        const NetworkMessage encoded =
            SnapshotSerializer::serialize(source);

        const auto framed =
            NetworkMessage::deserialize(
                encoded.serialize());

        assert(framed);

        const auto decoded =
            SnapshotDeserializer::deserialize(*framed);

        assert(decoded);
        assert(decoded->getCurrentTime() == source.getCurrentTime());
        assert(decoded->isGameOver() == source.isGameOver());
        assert(decoded->getSelectedPosition() == source.getSelectedPosition());
        assert(decoded->getPieces().size() == sourcePieces.size());

        for(std::size_t index = 0;
            index < sourcePieces.size();
            ++index)
        {
            assertPieceEqual(
                sourcePieces[index],
                decoded->getPieces()[index]);
        }
    }


    void verifyCommandResponseRoundTrip()
    {
        CommandResponse source;
        source.success = true;
        source.reason = "joined_room";
        source.command = "JOIN_ROOM";
        source.userId = "42";
        source.username = "alice";
        source.sessionId = "session-1";
        source.roomId = "room-1";
        source.side = Side::WHITE;
        source.selectedPosition = Position(1, 2);

        const auto decoded =
            CommandResponseCodec::deserialize(
                CommandResponseCodec::serialize(source));

        assert(decoded);
        assert(decoded->success == source.success);
        assert(decoded->reason == source.reason);
        assert(decoded->command == source.command);
        assert(decoded->userId == source.userId);
        assert(decoded->username == source.username);
        assert(decoded->sessionId == source.sessionId);
        assert(decoded->roomId == source.roomId);
        assert(decoded->side == source.side);
        assert(decoded->selectedPosition == source.selectedPosition);

        source.side = Side::OBSERVER;
        source.selectedPosition.reset();

        const auto decodedObserver =
            CommandResponseCodec::deserialize(
                CommandResponseCodec::serialize(source));

        assert(decodedObserver);
        assert(decodedObserver->side == Side::OBSERVER);
        assert(!decodedObserver->selectedPosition);

        CommandResponse clearSelection;
        clearSelection.reason = "invalid_click";
        clearSelection.command = "CLICK";

        const auto decodedClear =
            CommandResponseCodec::deserialize(
                CommandResponseCodec::serialize(clearSelection));

        assert(decodedClear);
        assert(!decodedClear->selectedPosition);
    }


    void verifyMalformedSnapshotsAreRejected()
    {
        const PieceSnapshot piece =
            makePiece(
                1,
                Side::WHITE,
                PieceType::KING,
                PieceState::IDLE);

        const GameSnapshot snapshot(
            std::vector<PieceSnapshot>{piece},
            1,
            false,
            Position(0, 0));

        const auto payload =
            SnapshotSerializer::serialize(snapshot)
                .getPayload();

        const auto expectRejected =
            [](nlohmann::json document)
            {
                assert(!SnapshotDeserializer::deserializePayload(
                    document.dump()));
            };

        nlohmann::json document =
            nlohmann::json::parse(payload);

        document["current_time"] = -1;
        expectRejected(document);

        document = nlohmann::json::parse(payload);
        document["selected_position"]["row"] = 8;
        expectRejected(document);

        document = nlohmann::json::parse(payload);
        document["pieces"][0]["row"] = -1;
        expectRejected(document);

        document = nlohmann::json::parse(payload);
        document["pieces"][0]["motion_destination"]["col"] = 8;
        expectRejected(document);

        document = nlohmann::json::parse(payload);
        document["pieces"][0]["animation_start_time"] = -1;
        expectRejected(document);

        document = nlohmann::json::parse(payload);
        document["pieces"][0]["motion_start_time"] = 10;
        document["pieces"][0]["motion_end_time"] = 9;
        expectRejected(document);

        assert(!SnapshotDeserializer::deserialize(
            NetworkMessage(
                MessageType::COMMAND_RESULT,
                payload)));
    }
}


int main()
{
    verifySnapshotRoundTrip();
    verifyCommandResponseRoundTrip();
    verifyMalformedSnapshotsAreRejected();

    assert(!CommandResponseCodec::deserialize("not-json"));
    assert(!SnapshotDeserializer::deserializePayload("{}"));

    return 0;
}
