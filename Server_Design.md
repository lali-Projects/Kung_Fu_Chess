# Kung Fu Chess — Cloud Scaling Architecture

## 1. Purpose

This document defines the proposed cloud and scaling architecture for **Kung Fu Chess**.

The architecture is designed to support:

- Many concurrent users and WebSocket connections.
- Many simultaneous games.
- High availability and low latency.
- Horizontal scaling across multiple game servers.
- Recovery from service and node failures.
- Preservation of the existing C++17 authoritative game architecture.

The design adds cloud infrastructure around the current system. It does not rewrite the existing `GameEngine`, `GameSession`, command pipeline, snapshot pipeline, or client rendering architecture.

---

## 2. Architectural Principles

### 2.1 The server remains authoritative

Clients send commands and receive authoritative state. Clients never directly mutate the game state.

### 2.2 One game has one authoritative owner

Each active room is owned by one authoritative execution path:

```text
Room
  → Game Server Shard
    → Game Worker
      → GameSession
        → GameEngine
```

Different games may run in parallel on different Workers and Game Servers, but one game is never actively owned by two servers at the same time.

### 2.3 Scaling happens across games

The system scales by adding:

- More API Gateway replicas.
- More WebSocket Gateway replicas.
- More Game Workers.
- More Game Server Shards.
- More cluster nodes.

The internal state of one `GameEngine` is not distributed across machines.

### 2.4 Real-time and non-real-time traffic are separated

- HTTPS/REST handles login, rooms, matchmaking, and history.
- WSS handles live commands, snapshots, reconnect, and state updates.

### 2.5 Cloud infrastructure does not know game rules

NGINX, Redis, NATS, PostgreSQL, Kubernetes, Gateways, and the Allocator do not decide whether a move is legal and do not modify board state.

---

## 3. Target Architecture

```text
                     +----------------------------+
                     |       Global Clients       |
                     +-------------+--------------+
                                   | (WSS / HTTPS)
                                   v
                     +----------------------------+
                     |       Edge / CDN / LB      |
                     +-------------+--------------+
                                   |
         +-------------------------+-------------------------+
         |                                                   |
         v                                                   v
+----------------------------+                  +----------------------------+
|     API Gateway (REST)     |                  |    WebSocket Gateways      |
|  (Auth, Rooms, History)    |                  |  (State Updates, Live)     |
+-------------+--------------+                  +-------------+--------------+
              |                                                   |
              +-------------------------+-------------------------+
                                        |
                                        v
                           +----------------------------+
                           |    Internal Messaging      |
                           | NATS Events / Request-Reply|
                           +-------------+--------------+
                                         |
               +-------------------------+-------------------------+
               |                         |                         |
               v                         v                         v
     +------------------+      +--------------------+     +----------------------+
     |   Matchmaker     |      |   Game Allocator   |     | Game Server Shards ×N|
     | (Player Matching)|      |  (Shard Selector)  |     | Authoritative Games  |
     +------------------+      +---------+----------+     +----------+-----------+
                                        |                           |
                                        v                           |
                              +----------------------+               |
                              |    Room Directory    |<--------------+
                              | (roomId -> owner)    |
                              +----------+-----------+
                                         |
                         +---------------+---------------+
                         |                               |
                         v                               v
              +----------------------+        +----------------------+
              |      PostgreSQL      |        |        Redis         |
              |   Durable Data       |        |   Temporary State    |
              +----------------------+        +----------------------+

              +------------------------------------------------------+
              |                    Observability                     |
              | Logs, Metrics, Tracing, Alerts, Health, Load Tests   |
              |          Connected to all system components          |
              +------------------------------------------------------+
```

---

## 4. Servers and System Components

## 4.1 Edge / Load Balancer

### Responsibility

- Provides the public entry point.
- Accepts HTTPS and WSS traffic.
- Terminates TLS.
- Distributes traffic across API and WebSocket Gateways.
- Removes unhealthy instances from routing.
- Exposes one logical endpoint to clients.

### Selected technology

**NGINX**

### Why it was selected

NGINX supports HTTP, HTTPS, WebSocket proxying, TLS termination, health-based routing, and load balancing. It is lightweight and fits both Docker Compose and Kubernetes deployments.

---

## 4.2 API Gateway

### Responsibility

Handles non-real-time operations:

- Registration.
- Login and logout.
- Room creation and room listing.
- Matchmaking requests.
- Game history and results.
- Request validation.
- Rate limiting.
- Routing to backend services.

It does not run games or process authoritative gameplay.

### Selected technology

**NGINX with C++17 backend services**

### Why it was selected

NGINX provides one public REST entry point, while C++17 keeps the backend compatible with the existing server code and models.

---

## 4.3 WebSocket Gateway

### Responsibility

- Maintains long-lived WSS connections.
- Associates connections with users, sessions, and rooms.
- Receives gameplay commands.
- Routes commands to the authoritative Game Server.
- Receives and distributes authoritative snapshots.
- Supports observers.
- Handles heartbeat, reconnect, bounded queues, and slow clients.

### Selected technology

**C++17 with IXWebSocket**

### Why it was selected

IXWebSocket is already used in the existing system. Keeping the Gateway in C++17 preserves protocol compatibility and avoids introducing a second real-time networking stack.

---

## 4.4 Matchmaker

### Responsibility

- Stores players waiting for automatic matching.
- Selects suitable opponents.
- Creates a match request.
- Removes matched or cancelled players from the queue.
- Publishes `MatchFound`.

Manual room creation does not require the Matchmaker.

### Selected technology

- **C++17**
- **Redis**
- **NATS**

### Why it was selected

C++17 preserves consistency with the server. Redis provides a fast temporary matchmaking queue. NATS publishes match events without tightly coupling the Matchmaker to the Allocator.

---

## 4.5 Game Allocator

### Responsibility

Selects the Game Server Shard that will own a new room.

Placement considers:

- Active games.
- CPU and memory.
- Connection count.
- Observer count.
- Outbound bandwidth.
- Worker and Tick delay.
- `ACTIVE` or `DRAINING` state.

The result is:

```text
roomId → gameServerId
```

### Selected technology

- **C++17**
- **Redis**
- **Prometheus**

### Why it was selected

C++17 integrates with the current system. Redis stores server registration and ownership data with low latency. Prometheus provides the load metrics used for placement.

---

## 4.6 Room Directory

### Responsibility

Stores the authoritative location of each active room:

```text
roomId → authoritative Game Server
```

It supports:

- Join routing.
- Observer routing.
- Reconnect routing.
- Room lifecycle lookup.
- Ownership leases.

It is authoritative for room location, not for game state or player roles.

### Selected technology

**Redis**

### Why it was selected

The directory is temporary, frequently updated, and read on latency-sensitive routing paths. Redis supports fast lookup, TTL, leases, and shared access by Gateways and Allocators.

---

## 4.7 Game Server Shards

### Responsibility

Game Server Shards run the authoritative games.

Each Shard contains:

- Local `RoomManager`.
- Local `EventBus`.
- Fixed Game Worker Pool.
- Multiple `GameSession`s.
- `CommandParser`.
- `CommandHandler`.
- `PlayerInputCoordinator`.
- `GameController`.
- `GameEngine`.
- `RealTimeArbiter`.
- `GameSnapshotBuilder`.
- Snapshot serialization and publication.

Each `GameSession` owns one game with WHITE, BLACK, observers, authoritative time, movement, jump, rest, captures, collisions, promotion, and game over.

### Selected technology

- **C++17**
- The existing Kung Fu Chess server.
- Local memory for live state.
- **Docker**
- **Kubernetes/K3s**

### Why it was selected

The existing server already contains the stable authoritative domain core. Keeping it preserves correctness, performance, and the existing architectural boundaries.

---

## 4.8 Game Workers

### Responsibility

Each Worker executes a fixed set of `GameSession`s.

```text
One GameSession → One Worker
```

The Worker:

- Processes commands in order.
- Advances authoritative time.
- Produces snapshots.
- Prevents concurrent mutation of the same game.
- Preserves deterministic ordering.

### Selected technology

**Fixed Worker Pool in C++17**

### Why it was selected

A fixed pool uses multiple CPU cores without creating one Thread per game. It reduces lock contention and makes game ownership explicit.

---

## 4.9 Internal Messaging

### Responsibility

NATS carries infrastructure and service communication such as:

- `MatchFound`
- `RoomCreated`
- `GameAllocated`
- `GameFinished`
- Server registration.
- Server draining.
- Request/Reply between services.
- Durable command replication for game recovery.

The following remain local to the Game Server:

- Every Tick.
- `GameEngine::wait`.
- Collision processing.
- Motion, jump, and rest simulation.
- Board state.

### Selected technology

**NATS Core and NATS JetStream**

### Why it was selected

NATS Core provides low-overhead Events and Request/Reply. JetStream provides durable replicated storage for messages that must survive failures, including the ordered command log used for game recovery.

---

## 4.10 PostgreSQL

### Responsibility

Stores durable system information such as:

- Accounts and credentials.
- Account status.
- Completed game results.
- Game history.
- Optional move history.
- Audit data.

It does not store live board state or every Tick.

### Selected technology

**PostgreSQL**

### Why it was selected

PostgreSQL provides durable relational storage, Transactions, Constraints, Indexes, shared access, replication, backups, and Point-in-Time Recovery.

---

## 4.11 Redis

### Responsibility

Stores shared temporary state such as:

- Sessions.
- Presence.
- Matchmaking queue.
- Room Directory.
- Room ownership leases.
- Reconnect information.
- Rate-limit counters.
- Temporary claims and locks.

It is not authoritative for live board state or game rules.

### Selected technology

**Redis**

### Why it was selected

Redis is suitable for temporary, frequently changing, low-latency data with TTL and atomic operations.

---

## 4.12 Observability

### Responsibility

Provides:

- Structured logs.
- Metrics.
- Distributed tracing.
- Dashboards.
- Alerts.
- Health checks.
- Capacity planning.
- Headless load testing.

Important measurements include:

- Active connections and rooms.
- Games per Worker.
- Tick duration and Tick delay.
- Command latency.
- Snapshot size and serialization time.
- Outbound bandwidth and queue size.
- CPU and memory.
- Redis, NATS, and PostgreSQL latency.
- Failover duration.

### Selected technologies

- **Prometheus**
- **Grafana**
- **Loki**
- **OpenTelemetry**
- **Alertmanager**
- Headless synthetic clients

### Why they were selected

Together they provide standard metrics, dashboards, logs, traces, alerts, and realistic load testing without requiring GUI clients.

---

## 5. Data Ownership and Storage

| Data | Authoritative owner | Storage |
|---|---|---|
| User account and credentials | Authentication | PostgreSQL |
| Active login session | Session layer | Redis |
| WebSocket connection | WebSocket Gateway | Gateway memory |
| Presence and connection location | Presence layer | Redis |
| Matchmaking queue | Matchmaker | Redis |
| Room location and ownership lease | Room Directory | Redis |
| WHITE / BLACK / OBSERVER roles | GameSession | Game Server memory |
| Board state | GameEngine | Game Server memory |
| Motion / Jump / Rest | GameEngine / RealTimeArbiter | Game Server memory |
| Authoritative game time | GameSession / GameEngine | Game Server memory |
| Recovery checkpoint | Game Server recovery layer | Durable checkpoint storage |
| Ordered command log | Game Server recovery layer | NATS JetStream |
| Completed game result | History/Persistence | PostgreSQL |
| Metrics | Observability | Prometheus |
| Logs | Observability | Loki |

---

## 6. Main Data Flows

## 6.1 Login

```text
Client
  → Edge / Load Balancer
  → API Gateway
  → Authentication
  → PostgreSQL
  → Redis Session
  → Token / Session returned
```

## 6.2 Manual room creation

```text
Client
  → API Gateway
  → Game Allocator
  → Selected Game Server
  → Create Room and GameSession
  → Register room in Room Directory
  → Return room assignment
```

## 6.3 Matchmaking

```text
Players
  → API Gateway
  → Matchmaker
  → Redis Queue
  → MatchFound through NATS
  → Game Allocator
  → Selected Game Server
  → Room created
```

## 6.4 Opening a game connection

```text
Client
  → WebSocket Gateway
  → Validate Session in Redis
  → Lookup room owner
  → Authoritative Game Server
  → GameSession validates user and role
  → Full authoritative Snapshot returned
```

## 6.5 Gameplay command

```text
Client
  → WebSocket Gateway
  → Authoritative Game Server
  → CommandParser
  → CommandHandler
  → Owning Game Worker
  → GameSession
  → PlayerInputCoordinator
  → GameController
  → GameEngine
```

## 6.6 State publication

```text
GameSession tick
  → GameEngine
  → GameSnapshotBuilder
  → SnapshotSerializer
  → WebSocket Gateway
  → Players and Observers
```

## 6.7 Game completion

```text
GameEngine detects game over
  → Final Snapshot
  → GameFinished through NATS JetStream
  → Persist result in PostgreSQL
  → Close room ownership
```

---

## 7. Backpressure

A slow connection must never block a Game Worker or another client.

The selected policy is:

- Every connection has bounded outbound message and byte limits.
- `COMMAND_RESULT` messages are kept in order.
- An obsolete queued `GAME_STATE` may be replaced by the newest Snapshot.
- One serialized Snapshot payload is reused for all recipients in the room.
- Slow observers do not delay players.
- A connection that remains over its queue limits is disconnected.
- After reconnect, the client receives a full authoritative Snapshot.

This policy preserves authoritative correctness while preventing unbounded memory growth.

---

## 8. Scaling Matrix

| Scaling need | Component that scales |
|---|---|
| More REST traffic | API Gateway and backend replicas |
| More live connections | WebSocket Gateway replicas |
| More concurrent games | Game Server Shards |
| More games per process | Game Workers |
| More matchmaking traffic | Matchmaker replicas |
| More durable database traffic | PostgreSQL capacity and replicas |
| More temporary-state traffic | Redis capacity and replicas |
| More observer fan-out | WebSocket Gateways and later spectator relays |

---

## 9. Failover Design

## 9.1 General policy

The selected failure policy is:

- Stateless services use Active-Active replicas.
- Stateful infrastructure uses Primary/Replica with automatic Failover.
- Kubernetes restarts failed Containers.
- Traffic returns only after Readiness succeeds.
- Replicas are spread across different Nodes.
- WebSocket clients reconnect after Gateway failure.
- Each game has one Primary and one Warm Standby.
- Game recovery uses an Authoritative Checkpoint and a Durable Ordered Command Log.
- Room ownership uses Lease and Fencing Epoch.
- A server with an old Epoch cannot publish State or accept Commands.

---

## 9.2 Edge / NGINX failure

### Design

At least two NGINX instances run behind a managed Cloud Load Balancer.

### Failure process

1. Health Check fails.
2. The failed instance is removed from routing.
3. New traffic is sent to a healthy NGINX instance.
4. Kubernetes restarts the failed instance.
5. It returns only after Readiness succeeds.
6. WebSocket clients affected by the failure reconnect through the public endpoint.

NGINX stores no authoritative application state.

---

## 9.3 API Gateway failure

### Design

API Gateway and REST services run as Active-Active Stateless replicas.

### Failure process

1. NGINX removes the failed replica.
2. New requests are routed to another replica.
3. Kubernetes restarts the failed instance.
4. Incomplete requests are retried.
5. Create operations use a Request ID to prevent duplicate execution.

Accounts and History remain in PostgreSQL; Sessions remain in Redis.

---

## 9.4 WebSocket Gateway failure

### Design

Multiple WebSocket Gateways run concurrently. Session and Presence are stored in Redis.

### Failure process

1. Connections owned by the failed Gateway close.
2. Clients reconnect with short Exponential Backoff.
3. NGINX routes them to another Gateway.
4. The Gateway validates the Session in Redis.
5. It resolves the current room owner.
6. `GameSession` revalidates the user and role.
7. The client receives a current full Snapshot.

The failed Gateway's local queue is not restored. Old state messages are unnecessary because the latest Snapshot restores the client.

---

## 9.5 Matchmaker failure

### Design

Multiple Matchmaker replicas share a Redis queue. Each match uses an atomic claim with TTL.

### Failure process

1. Another replica continues processing the queue.
2. Claims owned by the failed replica expire.
3. Unfinished players become available again.
4. Kubernetes restarts the failed replica.

No matchmaking state depends solely on local process memory.

---

## 9.6 Game Allocator failure

### Design

Multiple Allocator replicas share Allocation Requests and Ownership Claims in Redis.

### Failure process

1. Another replica reads the pending Allocation Request.
2. It verifies whether the target Game Server already created the room.
3. It completes the request or safely retries it.
4. Room ownership is registered only after Game Server confirmation.
5. Kubernetes restarts the failed Allocator.

Existing games continue because they do not depend on the Allocator after creation.

---

## 9.7 Room Directory / Redis failure

### Design

Redis runs with:

- Primary.
- Replica.
- Sentinel.
- AOF persistence.
- TTL-based Ownership Leases.
- Fencing Epochs.

### Failure process

1. Sentinel detects Primary failure.
2. Replica is promoted to Primary.
3. Services reconnect to the new Primary.
4. Game Servers renew Room Leases.
5. Gateways renew Presence.
6. Expired ownership records are removed automatically.
7. The failed Redis instance returns as a Replica.

During the short Failover window, active games continue locally, while Login, Join, Matchmaking, and Reconnect may pause.

---

## 9.8 Game Server failure

### Design

Each active game has:

- One Primary Game Server.
- One Warm Standby on another Node.
- A periodic Authoritative Checkpoint.
- A Durable Ordered Command Log in NATS JetStream.
- A Redis Ownership Lease and Fencing Epoch.

Only the Primary accepts authoritative commands and publishes state.

### Checkpoint content

The recovery checkpoint contains all information required to reconstruct `GameSession`, including:

- Full board and piece state.
- Piece IDs and states.
- Active Motion with timing, source, and destination.
- Active Jump and Rest.
- Authoritative game time.
- WHITE and BLACK assignments.
- Game status and game-over state.
- Last command sequence.
- Ownership Epoch.
- Internal `RealTimeArbiter` recovery data.

### Failure process

1. The Primary stops renewing its Lease.
2. Health monitoring confirms the server failure.
3. The Warm Standby is promoted.
4. A new Ownership Epoch is issued.
5. The Standby loads the latest Checkpoint.
6. It replays later commands from JetStream.
7. It restores the last acknowledged authoritative state.
8. Room Directory points to the new Primary.
9. Gateways route traffic to it.
10. Clients reconnect and receive a full Snapshot.
11. A new Warm Standby is assigned.
12. The failed server restarts without its old ownership.

### Command acknowledgement

A command is acknowledged only after it has:

1. Been accepted and processed by the Primary.
2. Been written to the Durable Ordered Command Log.
3. Received an authoritative sequence number.

This prevents an acknowledged command from being lost during Failover.

---

## 9.9 Game Worker failure

A severe Worker failure is treated as failure of the whole Game Server Process.

The Game Server Failover process promotes the Warm Standby and reconstructs the affected games. Kubernetes restarts the failed Process as an empty available server.

The system does not continue using a Process after a serious memory or internal consistency failure.

---

## 9.10 NATS failure

### Design

NATS runs as a three-node Cluster. JetStream data is replicated.

### Failure process

1. Clients reconnect to another NATS Node.
2. The Cluster continues while Quorum exists.
3. JetStream continues storing and delivering replicated messages.
4. Kubernetes restarts the failed Node.
5. The Node rejoins and synchronizes.

Game simulation continues locally. If JetStream loses Quorum, new game commands are not acknowledged until the durable log is available again.

---

## 9.11 PostgreSQL failure

### Design

PostgreSQL runs with:

- Primary.
- Synchronous Standby.
- Automatic Failover.
- Connection Pool.
- Daily Full Backup.
- Continuous WAL Archiving.
- Point-in-Time Recovery.

### Failure process

1. The Standby is promoted.
2. The logical database endpoint points to the new Primary.
3. The Connection Pool reconnects.
4. Interrupted Transactions fail and are retried using Request IDs.
5. The failed server is rebuilt as a new Standby.

Active games continue in Game Server memory. Result persistence resumes when PostgreSQL is available.

---

## 9.12 Observability failure

Observability runs with replicated components:

- Two Prometheus instances.
- Two Grafana replicas.
- Loki with shared Object Storage.
- Multiple OpenTelemetry Collectors.
- Alertmanager Cluster.

If one component fails, another replica continues. Kubernetes restarts the failed component.

Observability failure does not stop game simulation or authoritative networking.

---

## 9.13 Kubernetes and Node failure

### Selected cluster design

- Three Control Plane Nodes.
- Multiple Worker Nodes.
- Pod Anti-Affinity.
- Pod Disruption Budgets.
- Liveness and Readiness Probes.
- Persistent storage for Stateful infrastructure.

### Container failure

Kubernetes restarts the Container and restores traffic only after Readiness succeeds.

### Worker Node failure

1. Kubernetes marks the Node unavailable.
2. Stateless Pods are rescheduled.
3. WebSocket clients reconnect.
4. Games fail over to Warm Standbys on other Nodes.
5. Allocator stops selecting the failed Node.
6. Replacement Warm Standbys are created.

### Control Plane Node failure

The remaining two Control Plane Nodes maintain Quorum and continue operating the cluster.

---

## 9.14 Planned deployment and draining

Before updating a Game Server:

```text
ACTIVE
  → DRAINING
  → no new primary games
  → finish or promote existing games
  → shutdown
```

The Allocator stops assigning new games to the server. Short games may finish normally. Other games are transferred by promoting their Warm Standby and issuing a new Ownership Epoch.

The server is shut down only after it owns no Primary games.

---

## 10. Deployment Environment

### Development and integration testing

**Docker Compose** runs a small multi-container version of the architecture:

- NGINX.
- API Gateway.
- WebSocket Gateway.
- Matchmaker.
- Game Allocator.
- Multiple Game Servers.
- Redis.
- PostgreSQL.
- NATS.
- Prometheus and Grafana.

Docker Compose was selected because it provides simple local networking, dependencies, and repeatable multi-service testing.

### Production

**Kubernetes**, or **K3s** for a smaller installation, manages:

- Replicas.
- Restart.
- Service discovery.
- Rolling deployments.
- Health checks.
- Resource limits.
- Secrets and Configuration.
- Node scheduling.

Kubernetes manages infrastructure lifecycle. It does not decide Room ownership and does not replace the Game Allocator or `GameSession`.

---

## 11. Existing Components That Remain Unchanged

The following components remain fundamentally part of the existing game core:

- `GameEngine`
- `GameController`
- `GameSession`
- `GameContext`
- `PlayerInputCoordinator`
- `Board`
- `Piece`
- `RuleEngine`
- `RealTimeArbiter`
- `MotionManager`
- `JumpManager`
- `RestManager`
- `GameSnapshotBuilder`
- `GameSnapshot`
- `CommandParser`
- `CommandHandler`
- `RoomManager`
- `PlayerLifecycleService`
- `EventBus`

These components do not learn about Redis, PostgreSQL, NATS, Load Balancers, Shards, Nodes, or Kubernetes.

The cloud architecture wraps and coordinates the existing domain system rather than rewriting it.
