#include <array>
#include <chrono>
#include <cstdint>
#include <filesystem>
#include <future>
#include <iomanip>
#include <iostream>
#include <mutex>
#include <optional>
#include <random>
#include <sstream>
#include <stdexcept>
#include <string>
#include <system_error>
#include <thread>
#include <utility>
#include <vector>

#include <ixwebsocket/IXNetSystem.h>
#include <ixwebsocket/IXGetFreePort.h>

#include "Application.hpp"
#include "AuthenticationClient.hpp"
#include "AuthoritativeGameState.hpp"
#include "ClientCommandDispatcher.hpp"
#include "ClientSession.hpp"
#include "ClientSocket.hpp"
#include "CommandResponse.hpp"
#include "GameRenderer.hpp"
#include "GameSnapshot.hpp"
#include "GuiConfig.hpp"
#include "Img.hpp"
#include "Layout.hpp"
#include "MoveResult.hpp"
#include "PieceSnapshot.hpp"
#include "RoomClient.hpp"
#include "RemoteBoardClickSink.hpp"
#include "RemoteGameFrameSource.hpp"
#include "TextureManager.hpp"


namespace
{
    using namespace std::chrono_literals;


    [[noreturn]] void fail(const std::string& message)
    {
        throw std::runtime_error(message);
    }


    void require(bool condition, const std::string& message)
    {
        if(!condition)
        {
            fail(message);
        }
    }


    void requireSuccess(
        const MoveResult& result,
        const std::string& operation)
    {
        if(!result.success)
        {
            fail(
                operation +
                " failed: " +
                result.reason);
        }
    }


    void requireSuccess(
        const CommandResponse& response,
        const std::string& operation)
    {
        if(!response.success)
        {
            fail(
                operation +
                " failed: " +
                response.reason);
        }
    }


    template<typename Predicate>
    void requireEventually(
        Predicate predicate,
        std::chrono::milliseconds timeout,
        const std::string& description)
    {
        const auto deadline =
            std::chrono::steady_clock::now() + timeout;


        while(std::chrono::steady_clock::now() < deadline)
        {
            if(predicate())
            {
                return;
            }


            std::this_thread::sleep_for(10ms);
        }


        if(!predicate())
        {
            fail("Timed out waiting for " + description);
        }
    }


    std::uint64_t makeUniqueSeed()
    {
        const auto now =
            static_cast<std::uint64_t>(
                std::chrono::high_resolution_clock::now()
                    .time_since_epoch()
                    .count());


        std::random_device random;


        return
            now ^
            (static_cast<std::uint64_t>(random()) << 32U) ^
            static_cast<std::uint64_t>(random());
    }


    std::string makeSuffix(std::uint64_t seed)
    {
        std::ostringstream stream;
        stream
            << std::hex
            << std::setfill('0')
            << std::setw(12)
            << (seed & 0xFFFFFFFFFFFFULL);
        return stream.str();
    }


    class IxNetworkSystem
    {
    public:
        IxNetworkSystem()
        {
            if(!ix::initNetSystem())
            {
                fail("IXWebSocket network initialization failed");
            }
        }


        ~IxNetworkSystem()
        {
            ix::uninitNetSystem();
        }


        IxNetworkSystem(const IxNetworkSystem&) = delete;
        IxNetworkSystem& operator=(const IxNetworkSystem&) = delete;
    };


    class TemporaryDatabase
    {
    public:
        explicit TemporaryDatabase(const std::string& suffix)
            : m_path(
                std::filesystem::temp_directory_path() /
                ("kung_fu_chess_ws_" + suffix + ".db"))
        {
            std::error_code error;
            const bool alreadyExists =
                std::filesystem::exists(m_path, error);


            if(error)
            {
                fail(
                    "Unable to inspect temporary database path: " +
                    error.message());
            }


            require(
                !alreadyExists,
                "Temporary database name unexpectedly already exists");
        }


        ~TemporaryDatabase()
        {
            removeAndVerify();
        }


        bool removeAndVerify()
        {
            bool removed = true;


            for(const char* suffix :
                std::array<const char*, 4>{
                    "",
                    "-journal",
                    "-wal",
                    "-shm"})
            {
                const std::filesystem::path candidate(
                    m_path.string() + suffix);


                std::error_code error;
                std::filesystem::remove(
                    candidate,
                    error);


                if(error)
                {
                    removed = false;
                    continue;
                }


                const bool stillExists =
                    std::filesystem::exists(
                        candidate,
                        error);


                if(error || stillExists)
                {
                    removed = false;
                }
            }


            return removed;
        }


        const std::filesystem::path& path() const
        {
            return m_path;
        }


        TemporaryDatabase(const TemporaryDatabase&) = delete;
        TemporaryDatabase& operator=(const TemporaryDatabase&) = delete;


    private:
        std::filesystem::path m_path;
    };


    class TestClient
    {
    public:
        explicit TestClient(std::string name)
            : m_name(std::move(name)),
              dispatcher(socket, session, gameState),
              authentication(dispatcher, session),
              rooms(dispatcher, session)
        {
            dispatcher.setErrorHandler(
                [this](const std::string& reason)
                {
                    std::lock_guard<std::mutex> lock(m_errorMutex);
                    m_protocolErrors.push_back(reason);
                });
        }


        void connect(const std::string& url)
        {
            require(
                socket.connect(url),
                m_name + " failed to connect to " + url);


            requireEventually(
                [this]()
                {
                    return session.isConnected();
                },
                1s,
                m_name + " connected session state");
        }


        CommandResponse command(const std::string& text)
        {
            return dispatcher.sendCommandAndWait(text, 5s);
        }


        void disconnect()
        {
            socket.disconnect();
        }


        MoveResult joinRoom(const std::string& roomId)
        {
            gameState.clear();
            return rooms.joinRoom(roomId);
        }


        MoveResult leaveRoom()
        {
            MoveResult result = rooms.leaveRoom();
            if(result.success)
            {
                gameState.clear();
            }
            return result;
        }


        MoveResult logout()
        {
            MoveResult result = authentication.logout();
            if(result.success)
            {
                gameState.clear();
            }
            return result;
        }


        std::vector<std::string> protocolErrors() const
        {
            std::lock_guard<std::mutex> lock(m_errorMutex);
            return m_protocolErrors;
        }


        const std::string& name() const
        {
            return m_name;
        }


    private:
        std::string m_name;


    public:
        ClientSocket socket;
        ClientSession session;
        AuthoritativeGameState gameState;


    private:
        mutable std::mutex m_errorMutex;
        std::vector<std::string> m_protocolErrors;


    public:
        client::ClientCommandDispatcher dispatcher;
        AuthenticationClient authentication;
        RoomClient rooms;
    };


    bool piecesEqual(
        const PieceSnapshot& left,
        const PieceSnapshot& right)
    {
        return
            left.id == right.id &&
            left.side == right.side &&
            left.type == right.type &&
            left.position == right.position &&
            left.state == right.state &&
            left.animationStartTime == right.animationStartTime &&
            left.hasActiveAnimation == right.hasActiveAnimation &&
            left.hasMotion == right.hasMotion &&
            left.motionStart == right.motionStart &&
            left.motionDestination == right.motionDestination &&
            left.motionStartTime == right.motionStartTime &&
            left.motionEndTime == right.motionEndTime;
    }


    bool snapshotsEqual(
        const GameSnapshot& left,
        const GameSnapshot& right)
    {
        if(left.getCurrentTime() != right.getCurrentTime() ||
           left.isGameOver() != right.isGameOver() ||
           !(left.getSelectedPosition() ==
             right.getSelectedPosition()) ||
           left.getPieces().size() != right.getPieces().size())
        {
            return false;
        }


        for(std::size_t index = 0;
            index < left.getPieces().size();
            ++index)
        {
            if(!piecesEqual(
                    left.getPieces()[index],
                    right.getPieces()[index]))
            {
                return false;
            }
        }


        return true;
    }


    bool gameplayStateEqual(
        const GameSnapshot& left,
        const GameSnapshot& right)
    {
        if(left.isGameOver() != right.isGameOver() ||
           left.getPieces().size() != right.getPieces().size())
        {
            return false;
        }


        for(std::size_t index = 0;
            index < left.getPieces().size();
            ++index)
        {
            if(!piecesEqual(
                    left.getPieces()[index],
                    right.getPieces()[index]))
            {
                return false;
            }
        }


        return true;
    }


    bool hasSelection(
        RemoteGameFrameSource& frameSource,
        const Position& expected)
    {
        const auto snapshot = frameSource.getFrame(0);
        return
            snapshot &&
            snapshot->getSelectedPosition() &&
            *snapshot->getSelectedPosition() == expected;
    }


    bool hasNoSelection(
        RemoteGameFrameSource& frameSource)
    {
        const auto snapshot = frameSource.getFrame(0);
        return
            snapshot &&
            !snapshot->getSelectedPosition();
    }


    std::optional<GameSnapshot> activeWhitePawnMoveFrame(
        RemoteGameFrameSource& frameSource)
    {
        const auto snapshot = frameSource.getFrame(0);


        if(!snapshot)
        {
            return std::nullopt;
        }


        for(const PieceSnapshot& piece : snapshot->getPieces())
        {
            if(piece.side == Side::WHITE &&
               piece.type == PieceType::PAWN &&
               piece.position == Position(6, 0) &&
               piece.state == PieceState::MOVING &&
               piece.hasActiveAnimation &&
               piece.hasMotion &&
               piece.motionStart == Position(6, 0) &&
               piece.motionDestination == Position(5, 0) &&
               piece.motionEndTime > piece.motionStartTime)
            {
                return snapshot;
            }
        }


        return std::nullopt;
    }


    bool hasActiveWhitePawnMove(
        RemoteGameFrameSource& frameSource)
    {
        return activeWhitePawnMoveFrame(
            frameSource).has_value();
    }


    bool hasWhitePawnAtDestination(
        RemoteGameFrameSource& frameSource)
    {
        const auto snapshot = frameSource.getFrame(0);


        if(!snapshot)
        {
            return false;
        }


        for(const PieceSnapshot& piece : snapshot->getPieces())
        {
            if(piece.side == Side::WHITE &&
               piece.type == PieceType::PAWN &&
               piece.position == Position(5, 0))
            {
                return true;
            }
        }


        return false;
    }


    void requireNoProtocolErrors(const TestClient& client)
    {
        const auto errors = client.protocolErrors();


        if(errors.empty())
        {
            return;
        }


        std::ostringstream message;
        message
            << client.name()
            << " observed protocol errors:";


        for(const std::string& error : errors)
        {
            message << " " << error;
        }


        fail(message.str());
    }


    void registerAndLogin(
        TestClient& client,
        const std::string& username,
        const std::string& password)
    {
        requireSuccess(
            client.authentication.registerUser(
                username,
                password),
            client.name() + " registration");


        requireSuccess(
            client.authentication.login(
                username,
                password),
            client.name() + " login");


        require(
            client.session.isAuthenticated(),
            client.name() + " did not retain authenticated state");
        require(
            client.session.getUsername() == username,
            client.name() + " retained the wrong username");
        require(
            !client.session.getUserId().empty(),
            client.name() + " did not receive a user id");
        require(
            !client.session.getSessionId().empty(),
            client.name() + " did not receive a session id");
    }


    CommandResponse clickThroughGuiBoundary(
        TestClient& client,
        const Position& position)
    {
        RemoteBoardClickSink clickSink(client.dispatcher);
        auto completion =
            std::make_shared<std::promise<CommandResponse>>();
        auto result = completion->get_future();


        clickSink.setResultHandler(
            [completion](const CommandResponse& response)
            {
                completion->set_value(response);
            });


        clickSink.click(position);


        if(result.wait_for(5s) != std::future_status::ready)
        {
            fail("Timed out waiting for GUI click response");
        }


        return result.get();
    }


    void runFourClientWebSocketScenario()
    {
        IxNetworkSystem networkSystem;


        const std::uint64_t seed = makeUniqueSeed();
        const std::string suffix = makeSuffix(seed);
        const int availablePort = ix::getFreePort();
        require(
            availablePort > 0 && availablePort <= 65535,
            "IXWebSocket could not allocate an integration-test port");
        const std::uint16_t port =
            static_cast<std::uint16_t>(availablePort);
        const std::string url =
            "ws://127.0.0.1:" + std::to_string(port);
        const std::string roomId = "ws_room_" + suffix;
        const std::string password = "ws_test_password";


        TemporaryDatabase database(suffix);


        std::cout
            << "Starting four-client WebSocket integration scenario on "
            << url
            << " with temporary database "
            << database.path().string()
            << '\n';


        {
            Application application(
                port,
                database.path().string());
            application.start();


            {
                TestClient white("white client");
                TestClient black("black client");
                TestClient spectatorOne("spectator one");
                TestClient spectatorTwo("spectator two");
                TestClient replacement("replacement white client");


                white.connect(url);
                black.connect(url);
                spectatorOne.connect(url);
                spectatorTwo.connect(url);


                registerAndLogin(
                    white,
                    "ws_a_" + suffix,
                    password);
                registerAndLogin(
                    black,
                    "ws_b_" + suffix,
                    password);
                registerAndLogin(
                    spectatorOne,
                    "ws_c_" + suffix,
                    password);
                registerAndLogin(
                    spectatorTwo,
                    "ws_d_" + suffix,
                    password);


                requireSuccess(
                    white.rooms.createRoom(roomId),
                    "room creation");
                requireSuccess(
                    white.rooms.joinRoom(roomId),
                    "white room join");
                requireSuccess(
                    black.rooms.joinRoom(roomId),
                    "black room join");
                requireSuccess(
                    spectatorOne.rooms.joinRoom(roomId),
                    "first spectator room join");
                requireSuccess(
                    spectatorTwo.rooms.joinRoom(roomId),
                    "second spectator room join");


                require(
                    white.session.getRoomId() == roomId &&
                    white.session.getSide() == Side::WHITE,
                    "first JOIN_ROOM response did not carry WHITE metadata");
                require(
                    black.session.getRoomId() == roomId &&
                    black.session.getSide() == Side::BLACK,
                    "second JOIN_ROOM response did not carry BLACK metadata");
                require(
                    spectatorOne.session.getRoomId() == roomId &&
                    spectatorOne.session.getSide() == Side::OBSERVER,
                    "third JOIN_ROOM response did not carry OBSERVER metadata");
                require(
                    spectatorTwo.session.getRoomId() == roomId &&
                    spectatorTwo.session.getSide() == Side::OBSERVER,
                    "fourth JOIN_ROOM response did not carry OBSERVER metadata");


                requireEventually(
                    [&white,
                     &black,
                     &spectatorOne,
                     &spectatorTwo]()
                    {
                        const auto whiteSnapshot =
                            white.gameState.getSnapshot();
                        const auto blackSnapshot =
                            black.gameState.getSnapshot();
                        const auto spectatorOneSnapshot =
                            spectatorOne.gameState.getSnapshot();
                        const auto spectatorTwoSnapshot =
                            spectatorTwo.gameState.getSnapshot();


                        return
                            whiteSnapshot &&
                            blackSnapshot &&
                            spectatorOneSnapshot &&
                            spectatorTwoSnapshot &&
                            whiteSnapshot->getPieces().size() == 32 &&
                            blackSnapshot->getPieces().size() == 32 &&
                            spectatorOneSnapshot->getPieces().size() == 32 &&
                            spectatorTwoSnapshot->getPieces().size() == 32;
                    },
                    2s,
                    "all four initial authoritative snapshots");


                const auto initialAuthoritativeFrame =
                    white.gameState.getSnapshot();
                require(
                    initialAuthoritativeFrame.has_value(),
                    "missing authoritative frame for GUI adapter probe");


                AuthoritativeGameState frozenFrameState;
                frozenFrameState.update(
                    *initialAuthoritativeFrame);
                RemoteGameFrameSource frozenFrameSource(
                    frozenFrameState);
                const auto rendererFrame =
                    frozenFrameSource.getFrame(1000);
                require(
                    rendererFrame &&
                    snapshotsEqual(
                        *initialAuthoritativeFrame,
                        *rendererFrame),
                    "remote GUI frame source altered the exact "
                    "authoritative snapshot");


                const std::filesystem::path assetRoot(
                    KFC_TEST_ASSET_ROOT);
                require(
                    std::filesystem::is_directory(assetRoot),
                    "renderer smoke-test asset root is missing");


                Layout rendererLayout(
                    GuiConfig::WINDOW_WIDTH,
                    GuiConfig::WINDOW_HEIGHT,
                    8,
                    8);
                TextureManager rendererTextures;
                rendererTextures.loadBoardTexture(
                    (assetRoot / "board" / "board.png").string());
                rendererTextures.loadAllPieceAnimations(
                    rendererLayout.getCellSize(),
                    (assetRoot / "pieces").string());
                GameRenderer existingRenderer(
                    rendererLayout,
                    rendererTextures);
                Img rendererCanvas(
                    GuiConfig::WINDOW_WIDTH,
                    GuiConfig::WINDOW_HEIGHT);
                existingRenderer.render(
                    rendererCanvas,
                    *rendererFrame);
                require(
                    !rendererCanvas.empty() &&
                    rendererCanvas.width() == GuiConfig::WINDOW_WIDTH &&
                    rendererCanvas.height() == GuiConfig::WINDOW_HEIGHT,
                    "existing GameRenderer did not render the decoded "
                    "authoritative frame");


                RemoteGameFrameSource whiteFrameSource(
                    white.gameState);
                RemoteGameFrameSource blackFrameSource(
                    black.gameState);
                RemoteGameFrameSource spectatorOneFrameSource(
                    spectatorOne.gameState);
                RemoteGameFrameSource spectatorTwoFrameSource(
                    spectatorTwo.gameState);


                const int whiteInitialTime =
                    white.gameState.getSnapshot()->getCurrentTime();
                const int blackInitialTime =
                    black.gameState.getSnapshot()->getCurrentTime();
                const int spectatorOneInitialTime =
                    spectatorOne.gameState.getSnapshot()->getCurrentTime();
                const int spectatorTwoInitialTime =
                    spectatorTwo.gameState.getSnapshot()->getCurrentTime();


                requireEventually(
                    [&white,
                     &black,
                     &spectatorOne,
                     &spectatorTwo,
                     whiteInitialTime,
                     blackInitialTime,
                     spectatorOneInitialTime,
                     spectatorTwoInitialTime]()
                    {
                        const auto whiteSnapshot =
                            white.gameState.getSnapshot();
                        const auto blackSnapshot =
                            black.gameState.getSnapshot();
                        const auto spectatorOneSnapshot =
                            spectatorOne.gameState.getSnapshot();
                        const auto spectatorTwoSnapshot =
                            spectatorTwo.gameState.getSnapshot();


                        return
                            whiteSnapshot &&
                            blackSnapshot &&
                            spectatorOneSnapshot &&
                            spectatorTwoSnapshot &&
                            whiteSnapshot->getCurrentTime() >
                                whiteInitialTime &&
                            blackSnapshot->getCurrentTime() >
                                blackInitialTime &&
                            spectatorOneSnapshot->getCurrentTime() >
                                spectatorOneInitialTime &&
                            spectatorTwoSnapshot->getCurrentTime() >
                                spectatorTwoInitialTime;
                    },
                    2s,
                    "authoritative time to advance for players and spectators");


                const CommandResponse wrongSide =
                    clickThroughGuiBoundary(
                        white,
                        Position(1, 0));
                require(
                    !wrongSide.success,
                    "white client unexpectedly selected a black pawn");
                require(
                    wrongSide.reason ==
                        "piece_not_owned_by_player",
                    "wrong-side selection returned unexpected reason: " +
                        wrongSide.reason);
                require(
                    !wrongSide.selectedPosition,
                    "rejected wrong-side selection was not cleared");


                const auto stateBeforeSpectatorClicks =
                    white.gameState.getSnapshot();
                require(
                    stateBeforeSpectatorClicks.has_value(),
                    "missing authoritative state before spectator input");


                const CommandResponse spectatorWhiteClick =
                    clickThroughGuiBoundary(
                        spectatorOne,
                        Position(6, 0));
                require(
                    !spectatorWhiteClick.success &&
                    spectatorWhiteClick.reason == "observer_cannot_play" &&
                    !spectatorWhiteClick.selectedPosition,
                    "spectator unexpectedly selected a WHITE piece");


                const CommandResponse spectatorBlackClick =
                    clickThroughGuiBoundary(
                        spectatorTwo,
                        Position(1, 0));
                require(
                    !spectatorBlackClick.success &&
                    spectatorBlackClick.reason == "observer_cannot_play" &&
                    !spectatorBlackClick.selectedPosition,
                    "spectator unexpectedly selected a BLACK piece");


                requireEventually(
                    [&white,
                     &spectatorOneFrameSource,
                     &spectatorTwoFrameSource,
                     &stateBeforeSpectatorClicks]()
                    {
                        const auto stateAfterSpectatorClicks =
                            white.gameState.getSnapshot();


                        return
                            stateAfterSpectatorClicks &&
                            gameplayStateEqual(
                                *stateBeforeSpectatorClicks,
                                *stateAfterSpectatorClicks) &&
                            hasNoSelection(spectatorOneFrameSource) &&
                            hasNoSelection(spectatorTwoFrameSource);
                    },
                    1s,
                    "spectator input rejection without engine mutation or "
                    "selection leakage");


                const CommandResponse whiteSelection =
                    clickThroughGuiBoundary(
                        white,
                        Position(6, 0));
                requireSuccess(
                    whiteSelection,
                    "white pawn selection");
                require(
                    whiteSelection.reason == "piece_selected" &&
                    whiteSelection.selectedPosition &&
                    *whiteSelection.selectedPosition == Position(6, 0),
                    "white selection response did not contain (6,0)");


                const CommandResponse blackSelection =
                    clickThroughGuiBoundary(
                        black,
                        Position(1, 1));
                requireSuccess(
                    blackSelection,
                    "black pawn selection");
                require(
                    blackSelection.reason == "piece_selected" &&
                    blackSelection.selectedPosition &&
                    *blackSelection.selectedPosition == Position(1, 1),
                    "black selection response did not contain (1,1)");


                requireEventually(
                    [&whiteFrameSource,
                     &blackFrameSource,
                     &spectatorOneFrameSource,
                     &spectatorTwoFrameSource]()
                    {
                        return
                            hasSelection(
                                whiteFrameSource,
                                Position(6, 0)) &&
                            hasSelection(
                                blackFrameSource,
                                Position(1, 1)) &&
                            hasNoSelection(
                                spectatorOneFrameSource) &&
                            hasNoSelection(
                                spectatorTwoFrameSource);
                    },
                    1s,
                    "isolated player selections and empty spectator overlays");


                const CommandResponse malformedClick =
                    clickThroughGuiBoundary(
                        white,
                        Position(-1, -1));
                require(
                    !malformedClick.success &&
                    malformedClick.reason == "invalid_command" &&
                    hasSelection(
                        whiteFrameSource,
                        Position(6, 0)),
                    "an unparsed CLICK response desynchronized the retained "
                    "server selection from the client overlay");


                const CommandResponse nonClickResponse =
                    white.command("JOIN_ROOM " + roomId);
                require(
                    !nonClickResponse.success &&
                    nonClickResponse.reason == "player_cannot_join_room" &&
                    hasSelection(
                        whiteFrameSource,
                        Position(6, 0)),
                    "a non-CLICK command response cleared the active "
                    "selection overlay");


                std::promise<void> motionProbeStarted;
                auto motionProbeReady =
                    motionProbeStarted.get_future();
                auto activeMotionProbe =
                    std::async(
                        std::launch::async,
                        [&whiteFrameSource,
                         &blackFrameSource,
                         &spectatorOneFrameSource,
                         &spectatorTwoFrameSource,
                         &motionProbeStarted]()
                        {
                            motionProbeStarted.set_value();


                            std::optional<GameSnapshot>
                                whiteObservedFrame;
                            bool blackObserved = false;
                            bool spectatorOneObserved = false;
                            bool spectatorTwoObserved = false;
                            const auto deadline =
                                std::chrono::steady_clock::now() + 6s;


                            while(std::chrono::steady_clock::now() <
                                  deadline)
                            {
                                if(!whiteObservedFrame)
                                {
                                    whiteObservedFrame =
                                        activeWhitePawnMoveFrame(
                                            whiteFrameSource);
                                }
                                blackObserved =
                                    blackObserved ||
                                    hasActiveWhitePawnMove(
                                        blackFrameSource);
                                spectatorOneObserved =
                                    spectatorOneObserved ||
                                    hasActiveWhitePawnMove(
                                        spectatorOneFrameSource);
                                spectatorTwoObserved =
                                    spectatorTwoObserved ||
                                    hasActiveWhitePawnMove(
                                        spectatorTwoFrameSource);


                                if(whiteObservedFrame &&
                                   blackObserved &&
                                   spectatorOneObserved &&
                                   spectatorTwoObserved)
                                {
                                    return whiteObservedFrame;
                                }


                                std::this_thread::sleep_for(2ms);
                            }


                            return std::optional<GameSnapshot>{};
                        });


                require(
                    motionProbeReady.wait_for(1s) ==
                        std::future_status::ready,
                    "active-motion frame probe did not start");


                const CommandResponse legalMove =
                    clickThroughGuiBoundary(
                        white,
                        Position(5, 0));
                requireSuccess(
                    legalMove,
                    "white pawn move from (6,0) to (5,0)");
                require(
                    legalMove.reason == "ok",
                    "legal pawn move returned unexpected reason: " +
                        legalMove.reason);
                require(
                    !legalMove.selectedPosition,
                    "completed two-click move did not clear white selection");


                const auto activeRendererFrame =
                    activeMotionProbe.get();
                require(
                    activeRendererFrame.has_value(),
                    "all player and spectator GUI frame sources did not "
                    "expose the complete active-motion destination fields");


                existingRenderer.render(
                    rendererCanvas,
                    *activeRendererFrame);
                require(
                    !rendererCanvas.empty(),
                    "existing GameRenderer rejected the authoritative "
                    "active-motion frame");


                require(
                    hasSelection(
                        blackFrameSource,
                        Position(1, 1)) &&
                    hasNoSelection(spectatorOneFrameSource) &&
                    hasNoSelection(spectatorTwoFrameSource),
                    "white move disturbed isolated player/spectator "
                    "selection state");


                requireEventually(
                    [&whiteFrameSource,
                     &blackFrameSource,
                     &spectatorOneFrameSource,
                     &spectatorTwoFrameSource]()
                    {
                        return
                            hasWhitePawnAtDestination(whiteFrameSource) &&
                            hasWhitePawnAtDestination(blackFrameSource) &&
                            hasWhitePawnAtDestination(
                                spectatorOneFrameSource) &&
                            hasWhitePawnAtDestination(
                                spectatorTwoFrameSource);
                    },
                    4s,
                    "all players and spectators to receive the completed-move "
                    "snapshot");


                const int spectatorOneTimeBeforePeerDisconnect =
                    spectatorOne.gameState.getSnapshot()->getCurrentTime();


                spectatorTwo.disconnect();


                requireEventually(
                    [&spectatorTwo]()
                    {
                        return
                            !spectatorTwo.session.isConnected() &&
                            !spectatorTwo.session.isAuthenticated() &&
                            !spectatorTwo.session.hasRoom() &&
                            spectatorTwo.session.getSide() == Side::NONE &&
                            !spectatorTwo.gameState.hasSnapshot();
                    },
                    1s,
                    "disconnected spectator client state to clear");


                requireEventually(
                    [&spectatorOne,
                     spectatorOneTimeBeforePeerDisconnect]()
                    {
                        const auto snapshot =
                            spectatorOne.gameState.getSnapshot();


                        return
                            spectatorOne.session.isConnected() &&
                            spectatorOne.session.isAuthenticated() &&
                            spectatorOne.session.hasRoom() &&
                            spectatorOne.session.getSide() == Side::OBSERVER &&
                            snapshot &&
                            snapshot->getCurrentTime() >
                                spectatorOneTimeBeforePeerDisconnect;
                    },
                    2s,
                    "remaining spectator to stay subscribed after a "
                    "spectator disconnect");


                spectatorTwo.connect(url);


                MoveResult spectatorRelogin{
                    false,
                    "login_failed"};
                const auto spectatorReloginDeadline =
                    std::chrono::steady_clock::now() + 3s;


                do
                {
                    spectatorRelogin =
                        spectatorTwo.authentication.login(
                            "ws_d_" + suffix,
                            password);


                    if(spectatorRelogin.success)
                    {
                        break;
                    }


                    require(
                        spectatorRelogin.reason == "login_failed",
                        "spectator relogin failed unexpectedly: " +
                            spectatorRelogin.reason);


                    std::this_thread::sleep_for(25ms);
                }
                while(std::chrono::steady_clock::now() <
                      spectatorReloginDeadline);


                requireSuccess(
                    spectatorRelogin,
                    "spectator relogin after disconnect cleanup");
                requireSuccess(
                    spectatorTwo.joinRoom(roomId),
                    "spectator room rejoin after disconnect cleanup");
                require(
                    spectatorTwo.session.getRoomId() == roomId &&
                    spectatorTwo.session.getSide() == Side::OBSERVER,
                    "reconnected spectator did not recover OBSERVER metadata");


                requireEventually(
                    [&spectatorTwo]()
                    {
                        const auto snapshot =
                            spectatorTwo.gameState.getSnapshot();
                        return
                            snapshot &&
                            snapshot->getPieces().size() == 32;
                    },
                    2s,
                    "reconnected spectator's authoritative snapshot");


                white.disconnect();


                requireEventually(
                    [&white]()
                    {
                        return
                            !white.session.isConnected() &&
                            !white.session.isAuthenticated() &&
                            !white.session.hasRoom() &&
                            white.session.getSide() == Side::NONE;
                    },
                    1s,
                    "disconnected active-player session state to clear");
                requireEventually(
                    [&white]()
                    {
                        return !white.gameState.hasSnapshot();
                    },
                    1s,
                    "disconnected active-player authoritative state to clear");


                requireEventually(
                    [&black,
                     &spectatorOne,
                     &spectatorTwo,
                     &spectatorOneFrameSource,
                     &spectatorTwoFrameSource,
                     &roomId]()
                    {
                        return
                            black.session.isConnected() &&
                            black.session.getSide() == Side::BLACK &&
                            spectatorOne.session.isConnected() &&
                            spectatorOne.session.isAuthenticated() &&
                            spectatorOne.session.getRoomId() == roomId &&
                            spectatorOne.session.getSide() == Side::OBSERVER &&
                            spectatorTwo.session.isConnected() &&
                            spectatorTwo.session.isAuthenticated() &&
                            spectatorTwo.session.getRoomId() == roomId &&
                            spectatorTwo.session.getSide() == Side::OBSERVER &&
                            hasNoSelection(spectatorOneFrameSource) &&
                            hasNoSelection(spectatorTwoFrameSource);
                    },
                    2s,
                    "spectator memberships to remain stable after active-player "
                    "disconnect cleanup");


                const int spectatorOneWaitingTime =
                    spectatorOne.gameState.getSnapshot()->getCurrentTime();
                const int spectatorTwoWaitingTime =
                    spectatorTwo.gameState.getSnapshot()->getCurrentTime();


                replacement.connect(url);
                registerAndLogin(
                    replacement,
                    "ws_e_" + suffix,
                    password);
                requireSuccess(
                    replacement.joinRoom(roomId),
                    "replacement active-player join");
                require(
                    replacement.session.getRoomId() == roomId &&
                    replacement.session.getSide() == Side::WHITE,
                    "fresh client did not reclaim the vacant WHITE slot");
                require(
                    spectatorOne.session.getSide() == Side::OBSERVER &&
                    spectatorTwo.session.getSide() == Side::OBSERVER,
                    "an existing spectator was promoted when WHITE became "
                    "vacant");


                requireEventually(
                    [&replacement,
                     &spectatorOne,
                     &spectatorTwo,
                     spectatorOneWaitingTime,
                     spectatorTwoWaitingTime]()
                    {
                        const auto replacementSnapshot =
                            replacement.gameState.getSnapshot();
                        const auto spectatorOneSnapshot =
                            spectatorOne.gameState.getSnapshot();
                        const auto spectatorTwoSnapshot =
                            spectatorTwo.gameState.getSnapshot();


                        return
                            replacementSnapshot &&
                            spectatorOneSnapshot &&
                            spectatorTwoSnapshot &&
                            replacementSnapshot->getPieces().size() == 32 &&
                            spectatorOneSnapshot->getCurrentTime() >
                                spectatorOneWaitingTime &&
                            spectatorTwoSnapshot->getCurrentTime() >
                                spectatorTwoWaitingTime;
                    },
                    2s,
                    "replacement player and stable spectators to receive the "
                    "resumed authoritative stream");


                requireSuccess(
                    spectatorOne.leaveRoom(),
                    "spectator LEAVE_ROOM cleanup");
                require(
                    !spectatorOne.session.hasRoom() &&
                    spectatorOne.session.getSide() == Side::NONE &&
                    !spectatorOne.gameState.hasSnapshot(),
                    "spectator LEAVE_ROOM did not clear room, role, and state");
                requireSuccess(
                    spectatorOne.logout(),
                    "spectator logout after leave");
                require(
                    !spectatorOne.session.isAuthenticated(),
                    "spectator logout did not clear authentication state");


                requireSuccess(
                    spectatorTwo.logout(),
                    "in-room spectator LOGOUT cleanup");
                require(
                    !spectatorTwo.session.isAuthenticated() &&
                    !spectatorTwo.session.hasRoom() &&
                    spectatorTwo.session.getSide() == Side::NONE &&
                    !spectatorTwo.gameState.hasSnapshot(),
                    "in-room spectator LOGOUT did not clear identity, room, "
                    "role, and state");


                requireSuccess(
                    black.leaveRoom(),
                    "black room leave");
                requireSuccess(
                    black.logout(),
                    "black logout");
                require(
                    !black.session.isAuthenticated() &&
                    !black.session.hasRoom() &&
                    black.session.getSide() == Side::NONE &&
                    !black.gameState.hasSnapshot(),
                    "active-player leave/logout did not clear client state");


                requireSuccess(
                    replacement.leaveRoom(),
                    "replacement WHITE room leave");
                requireSuccess(
                    replacement.logout(),
                    "replacement WHITE logout");
                require(
                    !replacement.session.isAuthenticated() &&
                    !replacement.session.hasRoom() &&
                    replacement.session.getSide() == Side::NONE &&
                    !replacement.gameState.hasSnapshot(),
                    "replacement active-player cleanup left client state");


                black.disconnect();
                spectatorOne.disconnect();
                spectatorTwo.disconnect();
                replacement.disconnect();


                requireNoProtocolErrors(white);
                requireNoProtocolErrors(black);
                requireNoProtocolErrors(spectatorOne);
                requireNoProtocolErrors(spectatorTwo);
                requireNoProtocolErrors(replacement);
            }


            application.stop();
        }


        std::error_code databaseStatusError;
        const bool databaseWasCreated =
            std::filesystem::exists(
                database.path(),
                databaseStatusError);
        require(
            !databaseStatusError && databaseWasCreated,
            "temporary database was not present after application teardown");
        require(
            database.removeAndVerify(),
            "temporary database cleanup left a database or SQLite sidecar");


        std::cout
            << "Four-client WebSocket spectator scenario passed\n";
    }
}


int main()
{
    try
    {
        runFourClientWebSocketScenario();
        return 0;
    }
    catch(const std::exception& exception)
    {
        std::cerr
            << "Four-client WebSocket spectator scenario failed: "
            << exception.what()
            << '\n';
        return 1;
    }
    catch(...)
    {
        std::cerr
            << "Four-client WebSocket spectator scenario failed: "
            << "unknown exception\n";
        return 1;
    }
}
