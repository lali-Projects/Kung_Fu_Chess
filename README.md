# Kung Fu Chess

Real-time chess game implemented in C++17.

The project is built with a layered architecture that separates game logic, server communication, networking, and the graphical user interface (GUI). The architecture follows SOLID principles and keeps game rules completely independent from communication and presentation layers.

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
- Real-time command parsing and execution
- Game state broadcasting
- Game session management

#### Player Management
- **Player 1** -> WHITE
- **Player 2** -> BLACK
- **Additional players** -> OBSERVER

**Main Components:** `GameSession`, `SessionManager`, `PlayerSession`

---

## Command Flow

Client requests flow linearly through the architecture. The server itself does not contain game rules:

```text
GUI / MouseInput
    -> RemoteBoardClickSink
    -> ClientCommandDispatcher
    -> ClientSocket
    -> WebSocketServer
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
GameEngine / GameSession
    -> EventBus
    -> Server
    -> Clients
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
    -> SnapshotSerializer
    -> WebSocket
    -> SnapshotDeserializer
    -> AuthoritativeGameState
    -> Clients
```

This keeps network clients independent from internal C++ game objects.

The existing snapshot and animation model is preserved. The renderer consumes the same `GameSnapshot` / `PieceSnapshot` model used by the original local GUI flow.

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

Local mode is still supported through `LocalBoardClickSink` and `LocalGameFrameSource`.

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
* CMake 3.16+ (or 4.x)
* Ninja build system
* OpenCV
* IXWebSocket
* Qt runtime required by the current lobby integration

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
