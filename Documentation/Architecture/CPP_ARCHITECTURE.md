# C++ Core Architecture

## Overview

Soccer-Game core architecture follows professional AAA game development standards using Unreal Engine 5 best practices.

## Module Structure

### Core Module
Foundational systems that are essential to the game framework.

**Classes:**
- `ASoccerGameMode` - Main game mode, manages match flow
- `ASoccerPlayerController` - Player input and control
- `USoccerGameLogging` - Centralized logging system
- `USoccerGameSettings` - Global configuration management

### Game Module
Gameplay-specific logic and systems.

**Classes:**
- `ASoccerGameState` - Network replicated game state (reachable by all clients)
- `ABall` - Ball physics and interaction (to be implemented)
- `AGoal` - Goal detection and scoring (to be implemented)

### Characters Module
Player character and AI character systems.

**Classes:**
- `ASoccerPlayerCharacter` - Player controlled character
  - Position: Goalkeeper, Defender, Midfielder, Forward
  - Stamina system
  - Movement and animation
  - Ball interaction (kick, pass, head, jump)
- `ASoccerAICharacter` - AI controlled character (to be implemented)
- `ASoccerGoalkeeper` - Goalkeeper specialization (to be implemented)

### Physics Module
Ball physics and collision systems.

**Classes:**
- `UBallPhysicsComponent` - Advanced ball physics (Magnus effect, friction)
- `USoccerPhysicsCalculator` - Physics calculations utility

### Network Module
Multiplayer and backend communication.

**Classes:**
- `USoccerNetworkManager` - Network communication handler
- `USoccerBackendClient` - REST API client for backend

### UI Module
User interface systems.

**Classes:**
- `USoccerHUD` - Main game HUD
- `USoccerMenuWidget` - Menu system

### Audio Module
Sound and music systems.

**Classes:**
- `USoccerAudioManager` - Audio system manager

## Class Hierarchy

```
AActor
├── AGameModeBase
│   └── ASoccerGameMode              [Core/Match Management]
├── AGameState
│   └── ASoccerGameState             [Game/Network Replication]
├── ACharacter
│   ├── ASoccerPlayerCharacter       [Characters/Player]
│   ├── ASoccerAICharacter           [Characters/AI]
│   └── ASoccerGoalkeeper            [Characters/Goalkeeper]
├── APawn
│   └── APlayerController
│       └── ASoccerPlayerController  [Core/Input Control]
└── AActor
    ├── ABall                        [Game/Physics]
    └── AGoal                        [Game/Scoring]

UObject
├── UActorComponent
│   ├── UCharacterMovementComponent  [Base]
│   ├── UCameraComponent             [Base]
│   └── UBallPhysicsComponent        [Physics/Custom]
├── USoccerGameLogging              [Core/Utilities]
├── USoccerGameSettings             [Core/Configuration]
├── USoccerNetworkManager           [Network/Communication]
├── USoccerBackendClient            [Network/API]
├── USoccerAudioManager             [Audio/Manager]
└── USoccerHUD                      [UI/HUD]
```

## Data Flow Architecture

### Player Movement
```
Input (Controller)
    ↓
Enhanced Input System
    ↓
PlayerController
    ↓
PlayerCharacter (MoveForward/MoveRight)
    ↓
CharacterMovement
    ↓
Animation Update
    ↓
Network Replication (if multiplayer)
    ↓
Pixel Streaming (Browser)
```

### Ball Interaction
```
Player Input (Kick/Pass/Header)
    ↓
PlayerCharacter::KickBall()
    ↓
BallPhysicsComponent
    ↓
Physics Calculation (Magnus, Friction, etc.)
    ↓
Ball Position Update
    ↓
Network Replication
    ↓
Collision Detection (Goals, Players)
    ↓
Scoring/Event Notification
    ↓
Backend Update
```

### Match Management
```
GameMode::StartMatch()
    ↓
Initialize Teams & Players
    ↓
Spawn PlayerCharacters
    ↓
Register with GameState
    ↓
Match Loop (Tick)
    ├── Update Match Time
    ├── Check Match Conditions
    ├── Update Team Stats
    ├── Network Replication
    └── Broadcast Updates
    ↓
Match End (Time/Goals)
    ↓
Calculate Results
    ↓
Send to Backend
    ↓
Cleanup
```

## Key Interfaces

### INetworkReplicatable
```cpp
// Network replication interface (to be implemented)
class INetworkReplicatable
{
    virtual void OnReplicated() = 0;
    virtual void Serialize(FArchive& Ar) = 0;
};
```

### IPhysicsObject
```cpp
// Physics interaction interface (to be implemented)
class IPhysicsObject
{
    virtual void ApplyForce(FVector Force) = 0;
    virtual FVector GetVelocity() const = 0;
};
```

## Network Replication Strategy

### Authority
- **Server (Authority)**
  - GameMode - Controls game flow and rules
  - GameState - Maintains official game state
  - Ball - Authoritative position
  - Match Results

### Replication
- **GameState Properties** - Replicated to all clients
  - Team scores
  - Player list
  - Ball possession
  - Match elapsed time

### Communication
- **Player Input** - RPC to server
- **Position Updates** - Network replication with interpolation
- **Game Events** - Multicast to all players
- **Backend** - REST API for persistence

## Stamina System

The stamina system is designed to add strategic depth to player movement.

**Mechanics:**
- Max Stamina: 100.0
- Regeneration: 20.0/second (idle)
- Sprint Cost: 30.0/second (active)
- Recovery: Automatic when not sprinting
- Minimum for Sprint: Tied to CurrentStamina > 0

**Usage:**
- Movement: Unlimited (basic walk)
- Sprint: Consumes stamina at 30/sec
- Special Moves (Kick, Pass, Header, Jump): 2-10 stamina per action
- Goalkeeper: Different stamina model (faster recovery)

## Input Mapping

Uses Enhanced Input System (UE5 recommended approach):

**Default Mappings:**
- `MoveAction` → WASD
- `LookAction` → Mouse
- `SprintAction` → Left Shift
- `KickAction` → Space/Left Mouse
- `JumpAction` → Space/Alt

Configuration: Can be overridden in DefaultInput.ini

## Code Standards Applied

### Naming Conventions
- Classes: `ASoccerGameMode`, `USoccerGameSettings`
- Functions: `KickBall()`, `UpdateStamina()`
- Variables: `CurrentStamina`, `bIsMatchActive`, `TeamId`
- Booleans: `b` prefix → `bMatchActive`
- Enums: `E` prefix → `EPlayerPosition`

### Documentation
- Doxygen-style comments on classes and public functions
- Parameter descriptions
- Return value descriptions
- Blueprint exposure annotations

### Blueprint Integration
- `UFUNCTION(BlueprintCallable)` for exposed functions
- `UPROPERTY(BlueprintReadWrite)` for exposed properties
- `UCLASS(Blueprintable)` for classes that can be blueprinted

## Next Steps (Future Phases)

**PHASE 2 - Core Gameplay:**
- Implement ball physics component
- Create goal detection system
- Develop collision and interaction systems
- Implement camera system

**PHASE 3 - Characters & Animation:**
- Add animation state machines
- Implement motion matching
- Create goalie-specific mechanics
- Add player customization

**PHASE 4-12:**
- Advanced physics, AI systems, game modes
- UI/UX, audio, graphics, multiplayer
- Optimization and polish

## Build Configuration

The project builds with:
- **Compiler**: MSVC (Windows) or Clang (Linux)
- **Configuration**: Development/Shipping
- **Platform**: Win64, Linux
- **Optimization**: Full optimization in Shipping builds

Build with:
```bash
.\Build\Visual Studio Solution → F7 (Build)
Debug → F5 (Run)
```

---

**Version**: 1.0  
**Last Updated**: 2026-05-26
