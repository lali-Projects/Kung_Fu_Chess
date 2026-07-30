# Kung Fu Chess

Real-time chess game implemented in C++17.

The project is built with a layered architecture that separates game logic, the shared protocol, client coordination, the authoritative server, persistence, networking, and the graphical user interface (GUI). The architecture follows SOLID principles and keeps game rules completely independent from communication and presentation layers.

## Project Tree

```text
Kung_Fu_Chess_updated/
├── assets/
│   ├── board/
│   │   └── board.png
│   └── pieces/
│       └── <piece>/
│           └── states/<state>/
│               ├── config.json
│               └── sprites/
├── src/
│   ├── application/
│   │   └── Application.{hpp,cpp}
│   ├── client/
│   │   ├── main.cpp
│   │   ├── application/
│   │   │   └── ClientApplicationController.{hpp,cpp}
│   │   ├── auth/
│   │   │   └── AuthenticationClient.{hpp,cpp}
│   │   ├── commands/
│   │   │   ├── IClientCommandGateway.hpp
│   │   │   └── ClientCommandDispatcher.{hpp,cpp}
│   │   ├── console/
│   │   │   └── ConsoleClient.{hpp,cpp}
│   │   ├── game/
│   │   │   ├── RemoteBoardClickSink.{hpp,cpp}
│   │   │   └── RemoteGameFrameSource.{hpp,cpp}
│   │   ├── network/
│   │   │   ├── ITransport.hpp
│   │   │   └── ClientSocket.{hpp,cpp}
│   │   ├── room/
│   │   │   └── RoomClient.{hpp,cpp}
│   │   ├── session/
│   │   │   └── ClientSession.{hpp,cpp}
│   │   ├── snapshot/
│   │   │   ├── SnapshotDeserializer.{hpp,cpp}
│   │   │   └── tests/ProtocolCodecRoundTripTests.cc
│   │   └── state/
│   │       └── AuthoritativeGameState.{hpp,cpp}
│   ├── common/
│   │   ├── CommandResponse.hpp
│   │   ├── CommandResponseCodec.{hpp,cpp}
│   │   ├── MoveResult.hpp
│   │   ├── NetworkMessage.{hpp,cpp}
│   │   ├── PieceTypes.hpp
│   │   └── nlohmann/json.hpp
│   ├── gui/
│   │   ├── animation/
│   │   ├── config/
│   │   ├── graphics/
│   │   ├── input/
│   │   ├── lobby/
│   │   ├── loop/
│   │   ├── rendering/
│   │   ├── resources/
│   │   └── window/
│   ├── integration/
│   │   └── tests/TwoClientWebSocketIntegrationTests.cc
│   ├── logic/
│   │   ├── config/
│   │   ├── engine/
│   │   ├── input/
│   │   ├── io/
│   │   ├── model/
│   │   ├── realtime/
│   │   ├── rules/
│   │   ├── setup/
│   │   ├── snapshot/
│   │   │   ├── GameSnapshot.{hpp,cpp}
│   │   │   ├── GameSnapshotBuilder.{hpp,cpp}
│   │   │   └── PieceSnapshot.hpp
│   │   └── texttests/ScriptRunner.hpp
│   └── server/
│       ├── main.cpp
│       ├── Server.{hpp,cpp}
│       ├── auth/
│       │   └── AuthService.{hpp,cpp}
│       ├── commands/
│       │   ├── ClickCommand.{hpp,cpp}
│       │   ├── Command.{hpp,cpp}
│       │   ├── CommandHandler.{hpp,cpp}
│       │   └── CommandParser.{hpp,cpp}
│       ├── connection/
│       │   ├── ClientConnection.{hpp,cpp}
│       │   └── ConnectionManager.{hpp,cpp}
│       ├── context/
│       │   └── GameContext.{hpp,cpp}
│       ├── database/
│       │   ├── DatabaseInitializer.{hpp,cpp}
│       │   ├── IDatabase.hpp
│       │   └── SQLiteDatabase.{hpp,cpp}
│       ├── events/
│       │   ├── Event.hpp
│       │   ├── EventBus.{hpp,cpp}
│       │   └── GameStateChangedEvent.{hpp,cpp}
│       ├── model/
│       │   └── User.{hpp,cpp}
│       ├── network/
│       │   ├── INetworkServer.hpp
│       │   ├── SnapshotSerializer.{hpp,cpp}
│       │   └── WebSocketServer.{hpp,cpp}
│       ├── repository/
│       │   └── UserRepository.{hpp,cpp}
│       ├── room/
│       │   ├── Room.{hpp,cpp}
│       │   ├── RoomFactory.{hpp,cpp}
│       │   └── RoomManager.{hpp,cpp}
│       └── sessions/
│           ├── AuthoritativeGameLoop.{hpp,cpp}
│           ├── GameSession.{hpp,cpp}
│           ├── PlayerInputCoordinator.{hpp,cpp}
│           ├── PlayerLifecycleService.{hpp,cpp}
│           ├── PlayerSession.{hpp,cpp}
│           └── PlayerSessionManager.{hpp,cpp}
├── CMakeLists.txt
├── .gitignore
└── README.md
```

The `build/` directory and `kungfu_chess.db` are generated locally by the build and server runtime and are not part of the source layout above.

## Features Implemented

### Game Engine

* Board management
* Chess pieces logic & movement validation
* Real-time movement & motion physics
* Jump mechanic
* Piece captures & states
* Pawn promotion
* Game over handling
* Authoritative server-side game clock

Main Components: `GameEngine`, `RuleEngine`, `Board`, `Piece`, `RealTimeArbiter`, `MotionManager`, `JumpManager`, `PromotionHandler`, `GameOverHandler`

---

## Server & Networking

Powered by IXWebSocket for network communication.

- WebSocket server startup & connection handling
- Client connection & disconnection management
- Registration, login, logout, and active player sessions
- Room creation, joining, leaving, and observer membership
- Typed command parsing, validation, and execution
- Room-filtered game state broadcasting
- Authoritative game session timing

#### Player Management
- **Player 1** -> WHITE
- **Player 2** -> BLACK
- **Additional players** -> OBSERVER

**Main Components:** `Server`, `WebSocketServer`, `ConnectionManager`, `ClientConnection`, `RoomManager`, `GameSession`, `PlayerSessionManager`, `PlayerSession`, `AuthoritativeGameLoop`

---

## Command Flow

Client requests flow linearly through the architecture. The server itself does not contain game rules:

```text
GUI / MouseInput
    -> RemoteBoardClickSink
    -> ClientCommandDispatcher
    -> ClientSocket
    -> WebSocket
    -> WebSocketServer
    -> Server
    -> ConnectionManager
    -> ClientConnection
    -> CommandParser
    -> CommandHandler
    -> GameSession
    -> PlayerInputCoordinator
    -> GameController
    -> GameEngine
    -> RuleEngine
```

The server remains authoritative. Clients do not mutate the game state directly.

---

## Event System

The project uses the Observer Pattern through an EventBus:

```text
GameSession
    -> EventBus
    -> Server
    -> ConnectionManager
    -> Clients in the same room
```

Features:

* Game state change events
* Snapshot updates
* Instant client notifications
* Room-filtered broadcasts
* Authoritative periodic updates

---

## Snapshot System

The internal game state is decoupled from clients using serialized snapshots:

```text
GameEngine
    -> GameSnapshotBuilder
    -> GameSnapshot
    -> GameSession / EventBus
    -> Server
    -> SnapshotSerializer
    -> ConnectionManager
    -> WebSocket
    -> ClientSocket
    -> ClientCommandDispatcher
    -> SnapshotDeserializer
    -> AuthoritativeGameState
    -> RemoteGameFrameSource
    -> GameRenderer
```

This keeps network clients independent from internal C++ game objects.

The renderer consumes the current `GameSnapshot` / `PieceSnapshot` model directly.

No separate network animation system is used.

---

## GUI Architecture

The GUI is decoupled from the game logic and does not handle movement rules, validation, or game decisions. Rendering is driven purely by game snapshots.

Remote multiplayer input:

```text
Window / MouseInput
    -> IBoardClickSink
    -> RemoteBoardClickSink
    -> ClientCommandDispatcher
    -> Server
```

Remote multiplayer rendering:

```text
Server Snapshot
    -> SnapshotDeserializer
    -> AuthoritativeGameState
    -> RemoteGameFrameSource
    -> GameLoop
    -> GameRenderer
    -> Window
```

The current gameplay executable uses the remote adapters only. `RemoteBoardClickSink` sends requests to the authoritative server, and `RemoteGameFrameSource` exposes only received server snapshots to the renderer.

Components: `GameSnapshotBuilder`, `BoardRenderer`, `PieceRenderer`, `GameRenderer`, `TextureManager`, `Layout`, `Img`, `MouseInput`, `GameLoop`, `RemoteBoardClickSink`, `RemoteGameFrameSource`, `AuthoritativeGameState`

---

## Client Architecture

The client now supports the complete multiplayer lifecycle.

* Connection to the WebSocket server
* Registration and login
* Create room
* Join room
* Leave room
* Logout
* WHITE / BLACK / OBSERVER role handling
* Asynchronous `GAME_STATE` processing
* Command response handling
* Authoritative snapshot storage
* GUI startup by default
* Console mode through `--console`

Main Components: `ClientApplicationController`, `ClientSocket`, `ClientCommandDispatcher`, `AuthenticationClient`, `RoomClient`, `ClientSession`, `SnapshotDeserializer`, `AuthoritativeGameState`

---

## Observer Support

After WHITE and BLACK are assigned, additional authenticated clients may join the same room as OBSERVER.

Observers:

* Receive the initial authoritative snapshot
* Receive all subsequent `GAME_STATE` updates
* See real-time movement and animations
* Use the same `GameRenderer` as active players
* Have no gameplay selection state
* Cannot issue valid gameplay actions

Observer gameplay commands are rejected by the server before reaching `GameController` or `GameEngine`.

---

## Requirements

### Prerequisites

* C++17 compliant compiler (GCC / Clang / MSVC)
* CMake 3.16+
* Ninja build system when using the commands below
* OpenCV
* Qt6 development packages for Core, Gui, and Widgets
* Git and network access during the first configure so CMake can fetch IXWebSocket v11.4.5 and SQLite 3.45.2

IXWebSocket and the SQLite amalgamation are fetched automatically by the current `CMakeLists.txt`; they do not need to be installed separately.

### Recommended IDEs

* Visual Studio 2022
* Visual Studio Code

---

## Integration Tests

The project includes protocol and real WebSocket integration tests.

Current test targets:

```text
protocol_codec_tests
two_client_websocket_integration_tests
```

`protocol_codec_tests` verifies `NetworkMessage`, command-response JSON, and authoritative snapshot serialization/deserialization without starting a server or opening a WebSocket.

Despite its historical target name, `two_client_websocket_integration_tests` currently creates four simultaneous connected clients (WHITE, BLACK, and two OBSERVER clients), plus a later replacement client that reclaims a vacant active side. It uses the real production WebSocket, server, client, SQLite, snapshot, and rendering code rather than mocks.

The WebSocket integration test verifies the complete production communication flow, including:

* Registration and login
* Room creation and joining
* WHITE / BLACK assignment
* Multiple OBSERVER clients
* Initial 32-piece authoritative snapshot
* Server-owned game-time advancement
* Wrong-side selection rejection
* Independent WHITE / BLACK selection state
* GUI input through `RemoteBoardClickSink`
* Authoritative movement
* Snapshot serialization / deserialization
* Active-motion data delivery
* Rendering of deserialized snapshots
* Observer gameplay rejection
* Observer disconnect / reconnect / leave / logout cleanup
* Active-player disconnect handling
* Vacant-side reclamation by a newly joining client

---

# Build & Run Instructions

From the project root directory, run:

## Configure Project

```powershell
cmake -S . -B build -G Ninja
```

## Build Server and Client

```powershell
cmake --build build --target server client --parallel 4
```

## Build Server, Client, and Tests

```powershell
cmake --build build --target server client protocol_codec_tests two_client_websocket_integration_tests --parallel 4
```

The client build copies the current `assets/` directory beside the client executable automatically.

---

## Run the Server

Open the first PowerShell / terminal window from the project root:

```powershell
.\build\server.exe
```

Keep the server running while clients are connected.

---

## Run the First Client

Open a second PowerShell / terminal window:

```powershell
.\build\client.exe
```

The GUI launches by default.

Register or log in, then create or join a room.

The first active client in the room is assigned:

```text
WHITE
```

---

## Run the Second Client

Open another PowerShell / terminal window:

```powershell
.\build\client.exe
```

Log in and join the same room.

The second active client is assigned:

```text
BLACK
```

---

## Run an Observer

Open another PowerShell / terminal window:

```powershell
.\build\client.exe
```

Log in and join the same room.

If WHITE and BLACK are already occupied, the client is assigned:

```text
OBSERVER
```

Additional clients may join the same room as OBSERVER.

Observers see the live game and animations but cannot perform gameplay actions.

---

## Recommended Runtime Setup

```text
Terminal 1:
.\build\server.exe

Terminal 2:
.\build\client.exe
-> WHITE

Terminal 3:
.\build\client.exe
-> BLACK

Terminal 4:
.\build\client.exe
-> OBSERVER

Terminal 5+:
.\build\client.exe
-> OBSERVER
```

---

## Console Mode

The GUI is the default client mode.

To launch the previous console workflow:

```powershell
.\build\client.exe --console
```

---

## Run Tests

After building the test targets:

```powershell
ctest --test-dir build --output-on-failure
```

Run only the WebSocket integration test:

```powershell
ctest --test-dir build -R two_client_websocket_integration_tests --output-on-failure
```

---

## Find Executables

If the executable location is different in your environment:

```powershell
Get-ChildItem -Recurse build -Filter server.exe
```

```powershell
Get-ChildItem -Recurse build -Filter client.exe
```

---

## Manual Multiplayer Verification

1. Start the server.
2. Start Client 1 and join as WHITE.
3. Start Client 2 and join as BLACK.
4. Start Client 3 or more and join as OBSERVER.
5. Move a WHITE piece and verify that BLACK and all observers see the same movement and animation.
6. Move a BLACK piece and verify that WHITE and all observers receive the same authoritative state.
7. Verify that WHITE cannot control BLACK pieces.
8. Verify that BLACK cannot control WHITE pieces.
9. Verify that OBSERVER clients cannot perform gameplay actions.
10. Disconnect an observer and verify that the active match continues.
11. Disconnect an active player and verify that observers remain connected and a newly joining client can claim the vacant active side.

---

## Docker Compose Deployment

The Docker deployment runs the authoritative server and NGINX in containers while the existing GUI client remains a native Windows application.

### Deployment Architecture

```text
Windows GUI Clients
        -> WebSocket: ws://127.0.0.1:8080
NGINX Container
        -> private Docker network
Authoritative Game Server Container (game-server:8080)
        -> /data/kungfu_chess.db
Persistent SQLite Named Volume
```

NGINX is the only host-accessible entry point. It forwards WebSocket traffic to the private `game-server:8080` service without parsing commands, changing JSON, or interpreting game state. The game server remains authoritative, and the existing `TYPE|payload` command and snapshot protocol is unchanged.

The Compose deployment uses these images and persistent resources:

* Local game-server image: `kung-fu-chess-server:compose`
* NGINX image: `nginx:1.28.0-alpine`
* Private application network: `kung-fu-chess-private`
* SQLite volume: `kung-fu-chess-sqlite-data`

### Prerequisites

* Docker Desktop for Windows
* A running Docker Engine
* Docker Compose v2 or later
* The native Windows GUI prerequisites listed in the existing [Requirements](#requirements) section

The GUI client is built and executed on Windows; it is not built or run inside the Docker deployment.

Verify Docker before starting:

```powershell
docker --version
docker compose version
docker info
```

### Quick Start

From the project root:

```powershell
docker compose up --build -d
docker compose ps
```

Both `game-server` and `nginx` should report `healthy`. Native clients connect through NGINX at:

```text
ws://127.0.0.1:8080
```

The game server is not published directly to the Windows host.

### Starting Two GUI Clients

The verified native Windows client executable is:

```text
.\build\client.exe
```

Start two instances from PowerShell:

```powershell
Start-Process .\build\client.exe
Start-Process .\build\client.exe
```

Register or log in as two different users, then join the same room. The first active player is assigned WHITE and the second is assigned BLACK.

### Logs

Follow all Compose logs:

```powershell
docker compose logs -f
```

Follow one service:

```powershell
docker compose logs -f game-server
docker compose logs -f nginx
```

`Ctrl+C` stops log viewing; it does not stop the containers.

### Stop, Start, and Remove Containers

Stop existing containers without removing them:

```powershell
docker compose stop
```

Start previously stopped containers:

```powershell
docker compose start
```

Stop and remove the Compose containers and application network:

```powershell
docker compose down
```

`docker compose down` preserves the named SQLite volume. Recreate or start the deployment in the background with:

```powershell
docker compose up -d
```

Rebuild the game-server image before starting after source changes:

```powershell
docker compose up --build -d
```

### SQLite Persistence

The database path inside `game-server` is:

```text
/data/kungfu_chess.db
```

`/data` is backed by the named volume `kung-fu-chess-sqlite-data`. The database and registered users persist across container restarts, game-server recreation, and `docker compose down` followed by `docker compose up -d`.

> [!WARNING]
> `docker compose down -v` deletes the named SQLite volume. This permanently deletes the stored database and registered users.

### Environment Configuration

Compose starts with working defaults even when no local `.env` exists. Copy `.env.example` to `.env` only when local overrides are required. The local `.env` file is ignored by Git and must not contain committed secrets.

| Variable | Default | Meaning |
|---|---|---|
| `KFC_PUBLIC_PORT` | `8080` | Windows host port published by NGINX |
| `KFC_SERVER_PORT` | `8080` | Private game-server port inside Docker; keep this at 8080 for the inherited health check |
| `KFC_SERVER_BIND_ADDRESS` | `0.0.0.0` | Game-server listener address that permits access from the container network |
| `KFC_DATABASE_PATH` | `/data/kungfu_chess.db` | Database path inside the game-server container |

### Health and Status

Inspect service state:

```powershell
docker compose ps
```

Both services should show `healthy`. The NGINX-only readiness endpoint is:

```text
http://127.0.0.1:8080/nginx-health
```

The endpoint checks NGINX itself and is not forwarded to the game server. The game-server TCP health check runs privately inside its container.

### Troubleshooting

1. **Docker Desktop is not running**

   Verify the daemon before retrying:

   ```powershell
   docker info
   ```

2. **Port 8080 is already in use**

   Find the Windows process holding the configured public port and stop it before starting Compose:

   ```powershell
   Get-NetTCPConnection -LocalPort 8080 -State Listen
   ```

3. **A service remains unhealthy**

   ```powershell
   docker compose ps
   docker compose logs game-server
   docker compose logs nginx
   ```

4. **The GUI client cannot connect**

   Confirm both services are healthy and check the NGINX readiness endpoint. The client endpoint must be `ws://127.0.0.1:8080`.

   ```powershell
   Invoke-WebRequest http://127.0.0.1:8080/nginx-health
   ```

5. **NGINX cannot reach the game server**

   Validate configuration, private DNS, and upstream TCP access:

   ```powershell
   docker compose exec nginx nginx -t
   docker compose exec nginx getent hosts game-server
   docker compose exec nginx nc -z -w 2 game-server 8080
   ```

6. **SQLite permission errors**

   Inspect the non-root server identity, database ownership, and server logs:

   ```powershell
   docker compose exec game-server id
   docker compose exec game-server ls -ln /data/kungfu_chess.db
   docker compose logs game-server
   ```

7. **Rebuilding after source changes**

   ```powershell
   docker compose up --build -d
   ```

8. **Stale Compose containers**

   Recreate the deployment without deleting the database volume:

   ```powershell
   docker compose down
   docker compose up --build -d
   ```

9. **Viewing game-server logs**

   ```powershell
   docker compose logs -f game-server
   ```

10. **Viewing NGINX logs**

    ```powershell
    docker compose logs -f nginx
    ```

11. **HTTP-400 entries in game-server logs**

    The inherited raw TCP health check opens the listener without completing a WebSocket handshake. IXWebSocket may log an HTTP-400 handshake message for that probe. These entries are expected and are not game failures.

### Validation Record

Stage 3 automated two independent WebSocket clients through `ws://127.0.0.1:8080` and verified registration/login, room creation and joining, WHITE/BLACK assignment, `CLICK`, `COMMAND_RESULT`, `GAME_STATE`, unchanged framing, SQLite persistence, and graceful shutdown.

Manual GUI status: **USER-VERIFIED MANUAL PASS**. The user reported that two native Windows GUI windows opened and supported two-player operation through the same Compose endpoint. This user observation was not repeated by the automated Stage 4 audit.

### Current Limitations

* The GUI client remains a native Windows executable and is not containerized.
* TLS/WSS termination is not implemented; the local endpoint uses `ws://`.
* The deployment runs one authoritative game-server instance.
* SQLite is used for the current single-server deployment.
* The raw TCP game-server health check may produce harmless HTTP-400 handshake logs.
