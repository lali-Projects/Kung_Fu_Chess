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
│       │   ├── DatabaseConfiguration.hpp
│       │   ├── DatabaseInitializer.{hpp,cpp}
│       │   ├── IDatabase.hpp
│       │   ├── PostgreSQLDatabase.{hpp,cpp}
│       │   ├── SQLiteDatabase.{hpp,cpp}
│       │   └── tests/PostgreSQLDatabaseTests.cc
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
├── docker/
│   ├── nginx/nginx.conf
│   └── postgres/init-app-user.sh
├── .dockerignore
├── .env.example
├── .gitignore
├── CMakeLists.txt
├── Dockerfile
├── docker-compose.yml
└── README.md
```

The `build/` directory, local `.env`, SQLite database files, and Docker named-volume contents are generated locally and are not part of the tracked source layout above.

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
* Docker Desktop and Docker Compose for the PostgreSQL + NGINX deployment
* A native libpq installation is required only when explicitly building PostgreSQL support on Windows with `KFC_ENABLE_POSTGRESQL=ON`

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

## Choose a Runtime Mode

The project supports two valid runtime modes. They use the same client and the same WebSocket endpoint, but they use different server and persistence setups.

| Mode | Server process | Persistence | Infrastructure | Recommended use |
|---|---|---|---|---|
| **Native Windows** | `build/server.exe` | SQLite (`kungfu_chess.db`) | No Docker required | Local development, debugging, and native tests |
| **Docker Compose** | Dockerized `game-server` behind NGINX | PostgreSQL named volume | PostgreSQL + game-server + NGINX containers | Full current deployment and end-to-end multiplayer operation |

Both modes expose the client endpoint:

```text
ws://127.0.0.1:8080
```

> [!IMPORTANT]
> Run only one server mode at a time. The native server and NGINX both use port `8080`, so starting both simultaneously causes a port conflict.

### Native Windows Summary

```powershell
.\build\server.exe
Start-Process .\build\client.exe
Start-Process .\build\client.exe
```

This mode uses SQLite by default and does not require `.env`, PostgreSQL, NGINX, or Docker Compose.

### Docker Compose Summary

```powershell
docker compose up -d
docker compose ps
Start-Process .\build\client.exe
Start-Process .\build\client.exe
```

This mode requires a local `.env` with PostgreSQL passwords and is the recommended full deployment. Use `docker compose up --build -d` after server, CMake, Dockerfile, or dependency changes.

## Runtime Option 1 — Native Windows Server with SQLite

This mode runs the authoritative server directly on Windows and uses SQLite by default. It is independent from the Docker Compose deployment. Stop Compose before starting the native server.

From the project root directory, run:

### Configure Project

```powershell
cmake -S . -B build -G Ninja
```

### Build Server and Client

```powershell
cmake --build build --target server client --parallel 4
```

### Build Server, Client, and Tests

```powershell
cmake --build build --target server client protocol_codec_tests two_client_websocket_integration_tests --parallel 4
```

The client build copies the current `assets/` directory beside the client executable automatically.

---

### Run the Native SQLite Server

Open the first PowerShell / terminal window from the project root:

```powershell
.\build\server.exe
```

When `KFC_DATABASE_TYPE` is omitted, the native server uses SQLite and stores data in `kungfu_chess.db`. Keep the server running while clients are connected. Do not run Docker Compose at the same time because both modes use port `8080`.

---

### Run the First Client

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

### Run the Second Client

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

### Run an Observer

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

### Recommended Native Runtime Setup

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

### Console Mode

The GUI is the default client mode.

To launch the previous console workflow:

```powershell
.\build\client.exe --console
```

---

### Run Tests

After building the test targets:

```powershell
ctest --test-dir build --output-on-failure
```

Run only the WebSocket integration test:

```powershell
ctest --test-dir build -R two_client_websocket_integration_tests --output-on-failure
```

---

### Find Executables

If the executable location is different in your environment:

```powershell
Get-ChildItem -Recurse build -Filter server.exe
```

```powershell
Get-ChildItem -Recurse build -Filter client.exe
```

---

### Manual Native Multiplayer Verification

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

## Runtime Option 2 — Docker Compose with PostgreSQL and NGINX

This is the recommended full deployment. It runs PostgreSQL, the authoritative server, and NGINX in containers while the existing GUI client remains a native Windows application. Stop any native `server.exe` process before starting Compose.

### Deployment Architecture

```text
Windows GUI Clients
        -> WebSocket: ws://127.0.0.1:8080
NGINX Container
        -> private Docker network
Authoritative Game Server Container (game-server:8080)
        -> PostgreSQL (postgres:5432)
Persistent PostgreSQL Named Volume
```

NGINX is the only host-accessible entry point. It forwards WebSocket traffic to the private `game-server:8080` service without parsing commands, changing JSON, or interpreting game state. The game server remains authoritative, and the existing `TYPE|payload` command and snapshot protocol is unchanged. PostgreSQL stores persistent application data such as users; active boards, pieces, sessions, motion, timers, and snapshots remain in memory in the authoritative game server.

The Compose deployment uses these images and persistent resources:

* Local game-server image: `kung-fu-chess-server:compose`
* NGINX image: `nginx:1.28.0-alpine`
* PostgreSQL image: `postgres:17.10-bookworm`
* Private application network: `kung-fu-chess-private`
* PostgreSQL volume: `kung-fu-chess-postgres-data`

### Prerequisites

* Docker Desktop for Windows
* A running Docker Engine
* Docker Compose v2 or later
* The native Windows GUI prerequisites listed in the existing [Requirements](#requirements) section

The GUI client is built and executed on Windows; it is not built or run inside the Docker deployment.

Create an untracked local environment file before using Compose, then replace both PostgreSQL password placeholders with different values:

```powershell
Copy-Item .env.example .env
```

Do not commit `.env` or use the example passwords outside local development. The bootstrap administrator initializes PostgreSQL, while the game server receives only the dedicated non-superuser application's credentials.

Verify Docker before starting:

```powershell
docker --version
docker compose version
docker info
```

### Quick Start

For the first local Compose setup, create `.env`, open it, and replace both password placeholders with different local values:

```powershell
Copy-Item .env.example .env
notepad .env
```

Start the deployment from the project root:

```powershell
docker compose up --build -d
docker compose ps
```

For later runs, when the image does not need rebuilding:

```powershell
docker compose up -d
docker compose ps
```

`postgres` must become healthy before `game-server` starts, and `game-server` must become healthy before NGINX starts. All three services should report `healthy`. Native clients connect through NGINX at:

```text
ws://127.0.0.1:8080
```

PostgreSQL and the game server are not published to the Windows host. Only NGINX publishes the loopback port.

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
docker compose logs -f postgres
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

`docker compose down` preserves the named PostgreSQL volume. Recreate or start the deployment in the background with:

```powershell
docker compose up -d
```

Rebuild the game-server image before starting after source changes:

```powershell
docker compose up --build -d
```

Normal end-of-session sequence:

```powershell
Get-Process client -ErrorAction SilentlyContinue | Stop-Process
docker compose down
```

This closes the native clients and removes the Compose containers and network while preserving PostgreSQL data.

### PostgreSQL and SQLite Persistence

Docker Compose uses PostgreSQL as its persistence backend. PostgreSQL stores its data at the image-managed path:

```text
/var/lib/postgresql/data
```

That path is backed by `kung-fu-chess-postgres-data`. Registered users persist across PostgreSQL restarts, container recreation, and `docker compose down` followed by `docker compose up -d`.

SQLite remains the default backend for native/local server execution when `KFC_DATABASE_TYPE` is omitted. Existing local SQLite files, `kfc-game-data`, and `kung-fu-chess-sqlite-data` remain separate from PostgreSQL. Existing SQLite users are retained but are not automatically imported into PostgreSQL; any future data-transfer tool must be a separate, explicit operation.

> [!WARNING]
> `docker compose down -v` deletes the named PostgreSQL volume. This permanently deletes the PostgreSQL database and registered users. Use `docker compose down` without `-v` for normal removal.

### Environment Configuration

Compose requires separate `KFC_POSTGRES_ADMIN_PASSWORD` and `KFC_POSTGRES_PASSWORD` values from the untracked local `.env`. Other values have development defaults. The bootstrap administrator creates the dedicated application role only when a fresh PostgreSQL volume is initialized; the game server receives only the application role and password. The application accepts only `sqlite` and `postgresql`; unsupported values fail startup without falling back to SQLite.

| Variable | Default | Meaning |
|---|---|---|
| `KFC_PUBLIC_PORT` | `8080` | Windows host port published by NGINX |
| `KFC_SERVER_PORT` | `8080` | Private game-server port inside Docker; keep this at 8080 for the inherited health check |
| `KFC_SERVER_BIND_ADDRESS` | `0.0.0.0` | Game-server listener address that permits access from the container network |
| `KFC_DATABASE_TYPE` | `postgresql` in Compose; `sqlite` when omitted natively | Selected `IDatabase` backend |
| `KFC_POSTGRES_HOST` | `postgres` | Private Compose PostgreSQL service name |
| `KFC_POSTGRES_PORT` | `5432` | Private PostgreSQL port; it is not published to Windows |
| `KFC_POSTGRES_DATABASE` | `kungfu_chess` | PostgreSQL database name |
| `KFC_POSTGRES_ADMIN_USER` | `kfc_admin` | Bootstrap administrator used only inside the PostgreSQL container |
| `KFC_POSTGRES_ADMIN_PASSWORD` | no committed default | Required bootstrap password supplied through `.env`; use a value different from the application password |
| `KFC_POSTGRES_USER` | `kfc` | Dedicated non-superuser PostgreSQL role used by the game server |
| `KFC_POSTGRES_PASSWORD` | no committed default | Required application password supplied through `.env`; this is the only database password given to the game server |
| `KFC_DATABASE_PATH` | `kungfu_chess.db` natively | SQLite path used only when `KFC_DATABASE_TYPE=sqlite` |

### Health and Status

Inspect service state:

```powershell
docker compose ps
```

All three services should show `healthy`. The NGINX-only readiness endpoint is:

```text
http://127.0.0.1:8080/nginx-health
```

The endpoint checks NGINX itself and is not forwarded to the game server. PostgreSQL uses `pg_isready`; the game-server TCP health check runs privately inside its container.

Inspect private PostgreSQL readiness without publishing port 5432:

```powershell
docker compose exec postgres pg_isready -U kfc_admin -d kungfu_chess
```

### Troubleshooting

1. **Docker Desktop is not running**

   Verify the daemon before retrying:

   ```powershell
   docker info
   ```

2. **Port 8080 is already in use**

   The native server and the Compose NGINX service both use port `8080`. Ensure that only one runtime mode is active. Inspect the listener, native server process, and running containers:

   ```powershell
   Get-NetTCPConnection -LocalPort 8080 -State Listen
   Get-Process server -ErrorAction SilentlyContinue
   docker ps
   ```

   Stop the native server process or the conflicting container before starting the other mode.

3. **A service remains unhealthy**

   ```powershell
   docker compose ps
   docker compose logs postgres
   docker compose logs game-server
   docker compose logs nginx
   ```

4. **The GUI client cannot connect**

   Confirm all three services are healthy and check the NGINX readiness endpoint. The client endpoint must be `ws://127.0.0.1:8080`.

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

6. **PostgreSQL connection or authentication errors**

   Confirm `.env` exists, both password placeholders were replaced with different values, PostgreSQL is healthy, and the private service name is `postgres`:

   ```powershell
   docker compose logs postgres
   docker compose exec game-server id
   docker compose exec game-server getent hosts postgres
   docker compose exec game-server nc -z -w 2 postgres 5432
   docker compose logs game-server
   ```

   Invalid hosts, credentials, ports, unsupported backend names, or migration failures prevent the game server from starting successfully. Changing either password variable does not rotate credentials already stored in an existing PostgreSQL volume; rotate the corresponding role deliberately before changing `.env`. PostgreSQL passwords are not written to application logs, and PostgreSQL selection never falls back to a local SQLite file.

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

10. **Viewing PostgreSQL logs**

    ```powershell
    docker compose logs -f postgres
    ```

11. **Viewing NGINX logs**

    ```powershell
    docker compose logs -f nginx
    ```

12. **HTTP-400 entries in game-server logs**

    The inherited raw TCP health check opens the listener without completing a WebSocket handshake. IXWebSocket may log an HTTP-400 handshake message for that probe. These entries are expected and are not game failures.

### Validation Record

Stage 3 automated two independent WebSocket clients through `ws://127.0.0.1:8080` and verified registration/login, room creation and joining, WHITE/BLACK assignment, `CLICK`, `COMMAND_RESULT`, `GAME_STATE`, unchanged framing, SQLite persistence, and graceful shutdown.

Stage 5 repeats the authentication and two-client protocol flow through the same endpoint using PostgreSQL, and validates migration idempotency plus persistence across PostgreSQL recreation and Compose down/up.

Manual GUI status: **USER-VERIFIED MANUAL PASS**. The user reported that two native Windows GUI windows opened and supported two-player operation through the same Compose endpoint. This user observation was not repeated by the automated Stage 4 audit.

### Current Limitations

* The GUI client remains a native Windows executable and is not containerized.
* TLS/WSS termination is not implemented; the local endpoint uses `ws://`.
* The deployment runs one authoritative game-server instance.
* PostgreSQL stores persistent user data but does not store or recover active game state.
* Existing SQLite records are not automatically imported into PostgreSQL.
* The raw TCP game-server health check may produce harmless HTTP-400 handshake logs.
