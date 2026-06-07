# Changelog

All notable changes to the Soccer-Game project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [0.1.0] - 2026-05-26

### Added - STEP 4
- Core C++ game framework classes
- `ASoccerGameMode` - Match initialization, start, end, pause/resume
- `ASoccerGameState` - Network replicated game state with team stats
- `ASoccerPlayerCharacter` - Player character with movement and stamina system
- `ASoccerPlayerController` - Player control and input handling
- `USoccerGameLogging` - Centralized logging with severity levels
- `USoccerGameSettings` - Global configuration management
- Complete C++ implementation with documentation
- Enhanced Input System integration
- Network replication framework setup
- Stamina system for player fatigue management
- Match state tracking and team statistics
- Player registration and team management
- Blueprint integration for all major classes
- C++ Architecture documentation (CPP_ARCHITECTURE.md)

### Technical Details - STEP 4
- Language: C++ (UE5 standards)
- Classes: 6 core framework classes
- Files: 12 source files (6 headers + 6 implementations)
- Architecture: Module-based organization
- Network: Replication with authority model
- Input: Enhanced Input System (UE5 best practice)

### Added - STEP 5
- Shared utility helpers for math, strings, and physics
- `USoccerMathHelpers` - clamp, magnitude, kick velocity, time formatting
- `USoccerStringHelpers` - sanitize, title case, and label formatting
- `USoccerPhysicsHelpers` - drag, velocity clamp, and impact force calculations
- Runtime managers for gameplay support
- `USoccerPlayerManager` - player record registration, updates, and queries
- `USoccerTeamManager` - team metadata and stat updates
- `USoccerMatchManager` - match lifecycle, timing, and summary state
- Blueprint-friendly utility APIs and singleton runtime services

### Technical Details - STEP 5
- Language: C++ (UE5 standards)
- Classes: 6 utility/runtime classes
- Files: 12 source files (6 headers + 6 implementations)
- Architecture: UObject-based runtime tools
- Integration: reusable helpers for current and future gameplay systems

### Added - STEP 6
- Basic game loop and framework integration
- `ASoccerGameMode` now drives match lifecycle and time updates
- `ASoccerGameState` exposes match activity and elapsed time sync for replication
- `USoccerMatchManager` and `USoccerTeamManager` are initialized and used during gameplay flow
- Startup team registration and lifecycle sync for future gameplay/UI systems

### Technical Details - STEP 6
- Language: C++ (UE5 standards)
- Core systems: GameMode, GameState, MatchManager, TeamManager
- Verification: editor diagnostics reported no errors in modified files

### Added - STEP 3
- Unreal Engine 5 project configuration (SoccerGame.uproject)
- C++ build system configuration (SoccerGame.Build.cs, SoccerGameEditor.Build.cs)
- Complete project directory structure for UE5
- Plugins configured (Pixel Streaming, OnlineSubsystem, EnhancedInput)
- Backend Node.js project with Express API framework
- Frontend React project with Vite & TypeScript
- Web Architecture documentation (ARCHITECTURE.md)
- Backend setup guide with API endpoints
- Frontend setup guide with component structure
- Docker Compose configuration for full-stack deployment
- Backend Dockerfile with multi-stage build
- Frontend Dockerfile with Nginx reverse proxy
- Nginx configuration for API/WebSocket proxying
- MongoDB database service configuration
- Redis caching service configuration
- Development environment setup templates

### Added - STEP 2
- GitHub Actions CI/CD workflows (build verification, code quality, release management)
- Automated build verification system
- Commit message validation workflow
- Code quality checks and standards enforcement
- Requirements tracking validation
- Pull request template with checklist
- Issue templates (bug report, feature request)
- Git workflow documentation (.github/GIT_WORKFLOW.md)
- CI/CD pipeline documentation (.github/CI_CD_DOCUMENTATION.md)
- Automated documentation validation
- Requirements.txt status symbol system

### Added - STEP 1
- Initial project setup and structure
- Development roadmap (60-step plan)
- Development standards documentation
- Git configuration (.gitignore)
- Project documentation (README, CHANGELOG)
- Core project architecture files
- Contributing guidelines
- MIT License

### Technical Details - STEP 3
- Engine: Unreal Engine 5.4
- Backend Runtime: Node.js 18+
- Frontend Framework: React 18.2+
- Database: MongoDB 7.0
- Cache: Redis 7.0
- Containerization: Docker & Docker Compose
- Reverse Proxy: Nginx Alpine
- Build Tool (Frontend): Vite
- Streaming: Pixel Streaming over WebRTC

### Project Structure - STEP 3
```
soccer-game/
├── Source/SoccerGame/          # C++ game code
├── Content/                     # Game assets
├── Backend/                     # Node.js API server
├── Frontend/                    # React web app
├── Documentation/Architecture/  # Web architecture
├── docker-compose.yml          # Container orchestration
└── Configuration files
```

### Next Steps
- Create core game framework classes (STEP 4)
- Implement utility systems and tools (STEP 5)
- Begin gameplay foundation (PHASE 2)

---

### Previous Changes (STEP 1-2)
See above sections for full changelog of STEP 1 and STEP 2

---

## Version History

### [Unreleased]

### Added - STEP 7
- Vector-based movement input processing in `ASoccerPlayerCharacter`
- Tunable movement acceleration and deceleration settings
- Stable movement intent tracking for future animation and ball interaction layers
- Sprinting remains tied to stamina consumption and recovery

### Technical Details - STEP 7
- Language: C++ (UE5 standards)
- Core systems: PlayerCharacter movement handling
- Verification: editor diagnostics reported no errors in modified C++ files

### Added - STEP 8
- Ball physics engine with `ASoccerBall` and `UBallPhysicsComponent`
- Drag and speed clamping on the ball simulated each tick
- Kicks, passes, and headers resolved as physics impulses
- Nearest-ball detection integrated into `ASoccerPlayerCharacter`
- Collision impact response with velocity reflection

### Technical Details - STEP 8
- Language: C++ (UE5 standards)
- Core systems: Ball actor and physics component
- Verification: editor diagnostics reported no errors in modified C++ files

### Added - STEP 9
- `ASoccerPitch` actor with basic field plane and boundary wall structure
- Global pitch dimensions and boundary settings in `USoccerGameSettings`
- Configurable futsal court layout for future level and gameplay design
- Boundary objects ready for collision-based court containment

### Technical Details - STEP 9
- Language: C++ (UE5 standards)
- Core systems: Pitch actor and game settings integration
- Verification: editor diagnostics reported no errors in modified C++ files

---

**Note**: Version numbering follows Semantic Versioning (MAJOR.MINOR.PATCH)
