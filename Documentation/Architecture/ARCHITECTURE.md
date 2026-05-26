# Soccer-Game Web Architecture

## System Overview

Soccer-Game is a hybrid application consisting of:

1. **Unreal Engine 5** - 3D game engine (C++)
2. **Backend API** - Node.js/Express microservices
3. **Frontend Web** - React web interface
4. **Database** - MongoDB for persistence
5. **Streaming** - Pixel Streaming for browser gameplay

## Architecture Diagram

```
┌─────────────────────────────────────────────────────────────┐
│                     CLIENT BROWSER                         │
│  ┌──────────────────────────────────────────────────────┐  │
│  │                   React Frontend                    │  │
│  │  ┌─────────────────────────────────────────────────┐ │  │
│  │  │ Main Menu │ Dashboard │ Settings │ Live Game  │ │  │
│  │  └─────────────────────────────────────────────────┘ │  │
│  │  ┌─────────────────────────────────────────────────┐ │  │
│  │  │   WebSocket Connection & HTTP REST API         │ │  │
│  │  └─────────────────────────────────────────────────┘ │  │
│  │  ┌─────────────────────────────────────────────────┐ │  │
│  │  │   Pixel Streaming (WebRTC/VP9)                 │ │  │
│  │  │   Live Game Feed from UE5                      │ │  │
│  │  └─────────────────────────────────────────────────┘ │  │
│  └──────────────────────────────────────────────────────┘  │
└─────────────────────────────────────────────────────────────┘
         ↕ HTTP/WebSocket ↕              ↕ Pixel Streaming ↕
         
┌──────────────────────────────────────────────────────────────┐
│              BACKEND INFRASTRUCTURE                         │
│  ┌──────────────────────────────────────────────────────┐   │
│  │           Node.js/Express API Server               │   │
│  │                                                      │   │
│  │  ┌─────────────────┐  ┌──────────────────────────┐ │   │
│  │  │  Auth Service   │  │  Game Service            │ │   │
│  │  │  • JWT tokens   │  │  • Match management      │ │   │
│  │  │  • User login   │  │  • Player stats          │ │   │
│  │  │  • Registration │  │  • Match results         │ │   │
│  │  └─────────────────┘  └──────────────────────────┘ │   │
│  │                                                      │   │
│  │  ┌─────────────────┐  ┌──────────────────────────┐ │   │
│  │  │  Player Service │  │  Multiplayer Service     │ │   │
│  │  │  • Profiles     │  │  • Matchmaking           │ │   │
│  │  │  • Stats        │  │  • Room management       │ │   │
│  │  │  • Progression  │  │  • WebSocket events      │ │   │
│  │  └─────────────────┘  └──────────────────────────┘ │   │
│  │                                                      │   │
│  │  ┌─────────────────┐  ┌──────────────────────────┐ │   │
│  │  │  Stream Service │  │  Analytics Service       │ │   │
│  │  │  • Pixel Stream │  │  • Match analytics       │ │   │
│  │  │  • RTC Bridge   │  │  • Player performance    │ │   │
│  │  │  • UE5 Comm     │  │  • Usage metrics         │ │   │
│  │  └─────────────────┘  └──────────────────────────┘ │   │
│  └──────────────────────────────────────────────────────┘   │
│                         ↕                                   │
│  ┌──────────────────────────────────────────────────────┐   │
│  │           MongoDB Database                          │   │
│  │  • Users & Authentication                           │   │
│  │  • Player Profiles & Statistics                     │   │
│  │  • Match History & Results                          │   │
│  │  • Team Configuration                               │   │
│  │  • Tournaments & Leaderboards                       │   │
│  └──────────────────────────────────────────────────────┘   │
└──────────────────────────────────────────────────────────────┘
         ↕ gRPC/Sockets ↕
         
┌──────────────────────────────────────────────────────────────┐
│            UNREAL ENGINE 5 GAME SERVER                     │
│  ┌──────────────────────────────────────────────────────┐   │
│  │              SoccerGame Application                │   │
│  │                                                      │   │
│  │  ┌─────────────────┐  ┌──────────────────────────┐ │   │
│  │  │  Game Framework │  │  Player System           │ │   │
│  │  │  • GameMode     │  │  • Character Movement    │ │   │
│  │  │  • GameState    │  │  • Input Handling        │ │   │
│  │  │  • Match Loop   │  │  • Animation System      │ │   │
│  │  └─────────────────┘  └──────────────────────────┘ │   │
│  │                                                      │   │
│  │  ┌─────────────────┐  ┌──────────────────────────┐ │   │
│  │  │  Physics System │  │  AI System               │ │   │
│  │  │  • Ball Physics │  │  • AI Players            │ │   │
│  │  │  • Collisions   │  │  • Tactics               │ │   │
│  │  │  • Interactions │  │  • Decision Making       │ │   │
│  │  └─────────────────┘  └──────────────────────────┘ │   │
│  │                                                      │   │
│  │  ┌─────────────────┐  ┌──────────────────────────┐ │   │
│  │  │  Pixel Streamer │  │  Network Replication     │ │   │
│  │  │  • WebRTC       │  │  • Server-side state     │ │   │
│  │  │  • Encoding     │  │  • Client replication    │ │   │
│  │  │  • Broadcasting │  │  • Latency compensation  │ │   │
│  │  └─────────────────┘  └──────────────────────────┘ │   │
│  └──────────────────────────────────────────────────────┘   │
└──────────────────────────────────────────────────────────────┘
```

## Technology Stack

### Frontend (Browser Access)
- **Framework**: React 18.2+ with TypeScript
- **3D Visualization**: Three.js + React Three Fiber
- **Streaming Client**: Pixel Streaming WebRTC receiver
- **Styling**: Tailwind CSS + Framer Motion
- **Build Tool**: Vite
- **State Management**: Zustand
- **Testing**: Vitest + React Testing Library

### Backend (API Server)
- **Runtime**: Node.js 18+
- **Framework**: Express.js
- **Real-time Communication**: Socket.IO + WebSocket
- **Authentication**: JWT (JSON Web Tokens)
- **Database**: MongoDB
- **Logging**: Pino
- **API Documentation**: Swagger/OpenAPI
- **Testing**: Jest + Supertest

### Game Engine
- **Engine**: Unreal Engine 5 (C++)
- **Plugins**: 
  - Pixel Streaming (browser streaming)
  - OnlineSubsystem (networking)
  - EnhancedInput (input system)
- **Platform**: Windows 64-bit, Linux
- **Networking**: Server-side replication + Pixel Streaming

## Data Flow

### Authentication Flow
```
Client → Browser Login → Backend API → JWT Token → Redux Store
Client → Authenticated Requests → API (JWT validation) → Protected Resources
```

### Game Session Flow
```
Frontend: Initiate Match
  ↓
Backend: Create Match Room
  ↓
UE5: Launch Multiplayer Session
  ↓
Pixel Streaming: Start Broadcasting
  ↓
Frontend: Receive Stream + WebRTC Connection
  ↓
Multiplayer Gameplay via WebSocket Events
```

### Real-time Multiplayer
```
Player Input (Frontend) → WebSocket Event → Backend → UE5 Server
UE5 Game State Update → Network Replication → All Connected Clients
Pixel Stream Video/Audio → WebRTC → Browser Display
```

## API Endpoints (Rest)

### Authentication
```
POST   /api/auth/register      - User registration
POST   /api/auth/login         - User login
POST   /api/auth/refresh       - Refresh JWT token
POST   /api/auth/logout        - User logout
GET    /api/auth/verify        - Verify token
```

### Players
```
GET    /api/players/:id        - Get player profile
PUT    /api/players/:id        - Update profile
GET    /api/players/:id/stats  - Get player statistics
GET    /api/leaderboard        - Get global leaderboard
```

### Matches
```
POST   /api/matches            - Create match
GET    /api/matches/:id        - Get match details
PUT    /api/matches/:id        - Update match
GET    /api/matches/:id/stream - Get streaming URL
POST   /api/matches/:id/join   - Join match
```

### Game Sessions
```
GET    /api/sessions/:id       - Get session details
POST   /api/sessions/:id/start - Start session
POST   /api/sessions/:id/end   - End session
```

## WebSocket Events

### Client → Server (Incoming)
```
player:join        - Player joins match
player:move        - Player movement update
player:kick        - Player kicks ball
player:chat        - Chat message
match:pause        - Pause match
match:forfeit      - Forfeit match
```

### Server → Client (Outgoing)
```
match:state        - Game state update
player:update      - Player data update
ball:update        - Ball position/state
score:update       - Score change
match:end          - Match ended
notification       - System notification
```

## Deployment Architecture

### Development
```
Localhost:3000  → Frontend (React Dev Server)
Localhost:3001  → Backend API
Localhost:8015  → UE5 Pixel Streaming
```

### Production
```
Docker Container 1: Frontend (Nginx + React Build)
Docker Container 2: Backend API (Node.js)
Docker Container 3: UE5 Game Server (Containerized)
Docker Container 4: MongoDB (Database)
```

## Security Considerations

- **CORS**: Restricted to allowed origins
- **Authentication**: JWT tokens with refresh rotation
- **Password**: Bcrypt hashing with salt rounds (12)
- **HTTPS**: Required in production
- **WebSocket Security**: WSS (Secure WebSocket)
- **Rate Limiting**: API rate limiting per user/IP
- **Input Validation**: Express validator + sanitization
- **Environment Variables**: Sensitive data in .env files

## Performance Targets

- **API Response Time**: < 200ms (p99)
- **WebSocket Latency**: < 100ms
- **Pixel Stream Bitrate**: 5-15 Mbps (adaptive)
- **Frontend FPS**: 60 FPS (streaming display)
- **Server FPS**: 60+ FPS (UE5 game)
- **Database Query**: < 50ms (p99)

## Scalability Strategy

1. **Horizontal Scaling**: Load balancer for API servers
2. **Database Replication**: MongoDB replica sets
3. **Match Instances**: Distributed game server instances
4. **Caching**: Redis for session/state caching
5. **CDN**: Static assets distribution
6. **Message Queue**: RabbitMQ for async tasks

---

**Architecture Version**: 1.0  
**Last Updated**: 2026-05-26
