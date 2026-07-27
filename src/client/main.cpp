#include <algorithm>
#include <exception>
#include <filesystem>
#include <iostream>
#include <memory>
#include <mutex>
#include <string>
#include <utility>

#include <QApplication>
#include <QDialog>

#include <ixwebsocket/IXNetSystem.h>

#include "AuthenticationClient.hpp"
#include "AuthoritativeGameState.hpp"
#include "ClientApplicationController.hpp"
#include "ClientCommandDispatcher.hpp"
#include "ClientSession.hpp"
#include "ClientSocket.hpp"
#include "ConsoleClient.hpp"
#include "GameLoop.hpp"
#include "GameRenderer.hpp"
#include "GuiConfig.hpp"
#include "Layout.hpp"
#include "LobbyDialog.hpp"
#include "MouseInput.hpp"
#include "RemoteBoardClickSink.hpp"
#include "RemoteGameFrameSource.hpp"
#include "RoomClient.hpp"
#include "TextureManager.hpp"
#include "Window.hpp"


namespace
{
    constexpr const char* DEFAULT_SERVER_URL =
        "ws://127.0.0.1:8080";


    struct GameLoopConnectionState
    {
        explicit GameLoopConnectionState(GameLoop& loop)
            : gameLoop(&loop)
        {
        }

        std::mutex mutex;
        GameLoop* gameLoop;
    };


    class GameLoopConnectionGuard
    {
    public:
        GameLoopConnectionGuard(
            client::ClientCommandDispatcher& dispatcher,
            std::shared_ptr<GameLoopConnectionState> state)
            : m_dispatcher(dispatcher),
              m_state(std::move(state))
        {
        }

        ~GameLoopConnectionGuard()
        {
            deactivate();
        }

        void deactivate()
        {
            if(!m_active)
            {
                return;
            }

            m_dispatcher.setConnectionHandler(nullptr);
            {
                std::lock_guard<std::mutex> lock(m_state->mutex);
                m_state->gameLoop = nullptr;
            }
            m_active = false;
        }

    private:
        client::ClientCommandDispatcher& m_dispatcher;
        std::shared_ptr<GameLoopConnectionState> m_state;
        bool m_active{true};
    };


    int runConsoleClient()
    {
        ClientSocket socket;
        AuthoritativeGameState gameState;
        ClientSession session;
        client::ClientCommandDispatcher dispatcher(
            socket,
            session,
            gameState);
        AuthenticationClient authentication(
            dispatcher,
            session);

        if(!socket.connect(DEFAULT_SERVER_URL))
        {
            std::cerr << "[CLIENT] Connection failed\n";
            return 1;
        }

        ConsoleClient console(socket, authentication);
        console.run();
        socket.disconnect();
        return 0;
    }


    int runGuiClient(int argc, char* argv[])
    {
        QApplication application(argc, argv);

        ClientSocket socket;
        AuthoritativeGameState gameState;
        ClientSession session;
        client::ClientCommandDispatcher dispatcher(
            socket,
            session,
            gameState);
        AuthenticationClient authentication(
            dispatcher,
            session);
        RoomClient rooms(dispatcher, session);
        ClientApplicationController controller(
            socket,
            authentication,
            rooms,
            session,
            gameState);

        dispatcher.setErrorHandler(
            [](const std::string& reason)
            {
                std::cerr
                    << "[CLIENT PROTOCOL] "
                    << reason
                    << "\n";
            });

        bool lobbyAccepted = false;
        {
            LobbyDialog lobby(controller);
            lobbyAccepted =
                lobby.exec() == QDialog::Accepted;
        }
        if(!lobbyAccepted)
        {
            controller.disconnect();
            return 0;
        }

        Layout layout(
            GuiConfig::WINDOW_WIDTH,
            GuiConfig::WINDOW_HEIGHT,
            8,
            8);
        TextureManager textures;
        const std::filesystem::path assetsPath =
            std::filesystem::path(
                QApplication::applicationDirPath().toStdString()) /
            "assets";
        textures.loadBoardTexture(
            (assetsPath / "board" / "board.png").string());
        textures.loadAllPieceAnimations(
            layout.getCellSize(),
            (assetsPath / "pieces").string());

        const Side assignedSide =
            session.getSide();
        const bool observing =
            assignedSide == Side::OBSERVER;
        const bool gameplayInputAllowed =
            assignedSide == Side::WHITE ||
            assignedSide == Side::BLACK;

        GameRenderer renderer(layout, textures);
        Window window(
            observing
                ? std::string(GuiConfig::WINDOW_TITLE) + " - Spectator"
                : GuiConfig::WINDOW_TITLE);
        RemoteGameFrameSource frameSource(gameState);
        GameLoop gameLoop(
            frameSource,
            renderer,
            window,
            GuiConfig::WINDOW_WIDTH,
            GuiConfig::WINDOW_HEIGHT,
            GuiConfig::FPS);
        RemoteBoardClickSink clickSink(dispatcher);
        MouseInput mouseInput(clickSink, layout);

        clickSink.setResultHandler(
            [](const CommandResponse& response)
            {
                if(!response.success)
                {
                    std::cerr
                        << "[CLIENT CLICK] "
                        << response.reason
                        << "\n";
                }
            });
        if(observing)
        {
            std::cout
                << "[CLIENT ROOM] Joined as spectator; "
                << "gameplay input is disabled.\n";
        }
        else if(gameplayInputAllowed)
        {
            window.setMouseCallback(
                [&mouseInput](int x, int y)
                {
                    mouseInput.click(x, y);
                });
        }

        const auto loopConnectionState =
            std::make_shared<GameLoopConnectionState>(gameLoop);
        GameLoopConnectionGuard connectionGuard(
            dispatcher,
            loopConnectionState);
        dispatcher.setConnectionHandler(
            [loopConnectionState](
                client::TransportState state,
                const std::string& reason)
            {
                if(state != client::TransportState::FAILED &&
                   state != client::TransportState::DISCONNECTED)
                {
                    return;
                }

                {
                    std::lock_guard<std::mutex> lock(
                        loopConnectionState->mutex);
                    if(loopConnectionState->gameLoop)
                    {
                        loopConnectionState->gameLoop->stop();
                    }
                }
                std::cerr
                    << "[CLIENT CONNECTION] "
                    << (reason.empty() ? "disconnected" : reason)
                    << "\n";
            });

        if(!controller.isConnected())
        {
            gameLoop.stop();
        }
        gameLoop.run();

        connectionGuard.deactivate();
        window.setMouseCallback(nullptr);
        if(controller.isInRoom())
        {
            const MoveResult result = controller.leaveRoom();
            if(!result.success)
            {
                std::cerr
                    << "[CLIENT ROOM] "
                    << result.reason
                    << "\n";
            }
        }
        if(controller.isAuthenticated())
        {
            const MoveResult result = controller.logout();
            if(!result.success)
            {
                std::cerr
                    << "[CLIENT LOGOUT] "
                    << result.reason
                    << "\n";
            }
        }
        controller.disconnect();
        return 0;
    }
}


int main(int argc, char* argv[])
{
    const bool consoleMode =
        std::find_if(
            argv + 1,
            argv + argc,
            [](const char* argument)
            {
                return std::string(argument) == "--console";
            }) != argv + argc;

    ix::initNetSystem();

    int result = 0;
    try
    {
        result = consoleMode
            ? runConsoleClient()
            : runGuiClient(argc, argv);
    }
    catch(const std::exception& exception)
    {
        std::cerr
            << "[CLIENT ERROR] "
            << exception.what()
            << "\n";
        result = 1;
    }

    ix::uninitNetSystem();
    return result;
}
