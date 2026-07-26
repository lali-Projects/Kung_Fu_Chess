# Kung Fu Chess

Real-time chess game implemented in **C++17**.

The project is built with a layered architecture that separates game logic, server communication, networking, and the graphical user interface (GUI). The architecture follows **SOLID** principles and keeps game rules completely independent from communication and presentation layers.

---

## Features Implemented

### Game Engine
- Board management
- Chess pieces logic & movement validation
- Real-time movement & motion physics
- Jump mechanic
- Piece captures & states
- Pawn promotion
- Game over handling

**Main Components:**
`GameEngine`, `RuleEngine`, `Board`, `Piece`, `RealTimeArbiter`, `MotionManager`, `JumpManager`, `PromotionHandler`, `GameOverHandler`

---

### Server & Networking
Powered by **IXWebSocket** for network communication.

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
Client ──> WebSocketServer ──> CommandHandler ──> GameSession ──> GameController ──> GameEngine ──> RuleEngine
```

---

## Event System

The project uses the **Observer Pattern** through an `EventBus`:

```text
GameEngine ──> EventBus ──> Server ──> Clients
```

**Features:**
- Game state change events
- Snapshot updates
- Instant client notifications

---

## Snapshot System

The internal game state is decoupled from clients using serialized snapshots:

```text
GameEngine ──> GameSnapshotBuilder ──> Serialized Snapshot ──> Clients
```

This keeps network clients independent from internal C++ game objects.

---

## GUI Architecture

The GUI is decoupled from the game logic and does **not** handle movement rules, validation, or game decisions. Rendering is driven purely by game snapshots.

**Components:**
`GameSnapshotBuilder`, `BoardRenderer`, `PieceRenderer`, `GameRenderer`, `TextureManager`, `Layout`, `Img`

---

## Requirements

### Prerequisites
- **C++17** compliant compiler (GCC / Clang / MSVC)
- **CMake** 3.16+ (or 4.x)
- **Ninja** build system
- **OpenCV**
- **IXWebSocket**

### Recommended IDEs
- Visual Studio 2022
- Visual Studio Code

---

## Build & Run Instructions

From the project root directory, run:

```powershell
# Configure project
cmake -S . -B build -G Ninja

# Build project
cmake --build build

# Run application
.\build\app.exe
```

---

## Integration Tests

The project includes an end-to-end server integration test.

The test runner is implemented in `main.cpp` and verifies the complete server flow:

- GameEngine initialization
- GameSession creation
- WebSocket server startup
- Client connections
- Client command handling
- Move processing through GameEngine
- EventBus notifications
- Snapshot broadcasting
- Identical game state delivery to clients
- Client disconnect handling

Run:

```powershell
.\build\app.exe
```

---

