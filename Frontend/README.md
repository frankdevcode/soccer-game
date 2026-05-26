# Frontend Setup Guide

## Overview

The Soccer-Game frontend is a modern React web application providing:
- User interface for game access
- Player dashboard & profile management
- Live game streaming (Pixel Streaming)
- Match creation & matchmaking
- Statistics & leaderboards
- Real-time multiplayer experience

## Quick Start

### Prerequisites
- Node.js 18+ (LTS recommended)
- npm 9+
- Git

### Installation

```bash
cd Frontend
npm install
```

### Environment Variables

Create `.env.local` file:
```env
# API Configuration
VITE_API_URL=http://localhost:3001
VITE_API_TIMEOUT=10000

# WebSocket Configuration
VITE_WEBSOCKET_URL=ws://localhost:3001

# Pixel Streaming Configuration
VITE_STREAM_URL=http://localhost:8015
VITE_STREAM_PROTOCOL=http

# Application
VITE_APP_NAME=Soccer-Game
VITE_ENV=development
```

### Running Development

```bash
# Development server with HMR
npm run dev

# Production build
npm run build

# Preview production build
npm run preview

# Run tests
npm test
npm test:ui

# Code linting
npm run lint

# Format code
npm run format

# Type checking
npm run type-check
```

## Project Structure

```
Frontend/
├── src/
│   ├── index.css              # Global styles
│   ├── App.jsx                # Root component
│   ├── main.jsx               # Entry point
│   ├── pages/
│   │   ├── Home.jsx           # Home page
│   │   ├── Login.jsx          # Login page
│   │   ├── Dashboard.jsx      # Player dashboard
│   │   ├── Profile.jsx        # Player profile
│   │   ├── Matches.jsx        # Matches list
│   │   ├── LiveGame.jsx       # Live game viewer
│   │   └── Leaderboard.jsx    # Leaderboard
│   ├── components/
│   │   ├── common/
│   │   │   ├── Header.jsx
│   │   │   ├── Footer.jsx
│   │   │   ├── Navigation.jsx
│   │   │   └── Loading.jsx
│   │   ├── game/
│   │   │   ├── StreamViewer.jsx    # Pixel Streaming display
│   │   │   ├── GameControls.jsx    # Game input controls
│   │   │   └── GameHUD.jsx         # In-game HUD overlay
│   │   ├── match/
│   │   │   ├── MatchCard.jsx
│   │   │   ├── MatchList.jsx
│   │   │   └── MatchCreator.jsx
│   │   └── player/
│   │       ├── PlayerCard.jsx
│   │       ├── PlayerStats.jsx
│   │       └── ProfileEditor.jsx
│   ├── stores/
│   │   ├── authStore.js       # Authentication state
│   │   ├── playerStore.js     # Player data state
│   │   ├── matchStore.js      # Match state
│   │   └── uiStore.js         # UI state
│   ├── services/
│   │   ├── api.js             # API client (axios)
│   │   ├── websocket.js       # WebSocket client
│   │   ├── streamClient.js    # Pixel Streaming client
│   │   └── auth.js            # Authentication service
│   ├── hooks/
│   │   ├── useAuth.js         # Auth hook
│   │   ├── useMatch.js        # Match hook
│   │   └── useStream.js       # Streaming hook
│   ├── utils/
│   │   ├── constants.js
│   │   ├── formatters.js
│   │   ├── validators.js
│   │   └── helpers.js
│   ├── styles/
│   │   ├── tailwind.config.js
│   │   ├── globals.css
│   │   └── animations.css
│   └── assets/
│       ├── images/
│       ├── icons/
│       └── fonts/
├── tests/
│   ├── unit/
│   ├── integration/
│   └── e2e/
├── public/
│   └── favicon.ico
├── vite.config.js
├── vitest.config.js
├── tailwind.config.js
├── postcss.config.js
├── tsconfig.json
├── .env.example
├── package.json
└── README.md
```

## Key Features

### Authentication
- User registration & login
- JWT token management
- Session persistence
- Automatic token refresh

### Player Management
- Profile creation & editing
- Player statistics tracking
- Progression system
- Team management

### Match System
- Create & join matches
- Matchmaking algorithm
- Match history
- Statistics recording

### Live Streaming
- Pixel Streaming integration
- Real-time game view
- WebRTC video stream
- Low-latency display

### Real-time Communication
- WebSocket events
- Live notifications
- Chat system
- Match updates

## Component Hierarchy

```
App
├── Router
│   ├── Home
│   ├── Login/Register
│   ├── Dashboard
│   │   ├── PlayerCard
│   │   ├── MatchList
│   │   └── Statistics
│   ├── LiveGame
│   │   ├── StreamViewer (Pixel Streaming)
│   │   ├── GameControls
│   │   └── GameHUD
│   ├── Matches
│   │   ├── MatchCard
│   │   ├── MatchCreator
│   │   └── Filters
│   └── Leaderboard
│       └── PlayerRanking
├── Navigation
├── Header
└── Footer
```

## State Management (Zustand)

### Auth Store
```javascript
const useAuthStore = create((set) => ({
  user: null,
  token: null,
  isLoggedIn: false,
  login: (user, token) => set({ user, token, isLoggedIn: true }),
  logout: () => set({ user: null, token: null, isLoggedIn: false })
}));
```

### Match Store
```javascript
const useMatchStore = create((set) => ({
  matches: [],
  currentMatch: null,
  loading: false,
  fetchMatches: async () => { /* ... */ },
  joinMatch: async (id) => { /* ... */ }
}));
```

## API Integration

### Example API Call
```javascript
import { useQuery, useMutation } from '@tanstack/react-query';
import api from '@/services/api';

function PlayerProfile({ playerId }) {
  const { data: player, isLoading } = useQuery(
    ['player', playerId],
    () => api.get(`/players/${playerId}`),
    { staleTime: 5 * 60 * 1000 } // 5 minutes
  );
  
  const mutation = useMutation(
    (data) => api.put(`/players/${playerId}`, data)
  );
  
  return (
    // Component JSX
  );
}
```

## WebSocket Integration

```javascript
import { useEffect } from 'react';
import { ws } from '@/services/websocket';

function LiveGame() {
  useEffect(() => {
    ws.on('match:state', (data) => {
      // Update game state
    });
    
    return () => ws.off('match:state');
  }, []);
}
```

## Pixel Streaming Integration

```javascript
import StreamViewer from '@/components/game/StreamViewer';

function LiveGame() {
  return (
    <StreamViewer
      url={process.env.VITE_STREAM_URL}
      width="100%"
      height="600px"
      onConnect={() => console.log('Connected')}
      onDisconnect={() => console.log('Disconnected')}
    />
  );
}
```

## Testing

```bash
# Unit tests
npm test

# Watch mode
npm test:watch

# UI mode
npm test:ui

# Coverage
npm test -- --coverage
```

## Build & Deployment

### Development Build
```bash
npm run dev
```

### Production Build
```bash
npm run build

# Check production build
npm run preview
```

### Docker Deployment
```bash
# Build image
docker build -t soccer-game-frontend .

# Run container
docker run -p 3000:80 soccer-game-frontend
```

### Deployment Checklist
- [ ] Environment variables configured
- [ ] API endpoints updated for production
- [ ] Security headers configured
- [ ] CORS properly set
- [ ] SSL/HTTPS enabled
- [ ] Performance optimized
- [ ] Error tracking configured
- [ ] Monitoring enabled

## Performance Optimization

1. **Code Splitting**: Route-based lazy loading
2. **Image Optimization**: WebP format with fallbacks
3. **Bundle Minification**: Production build optimization
4. **Caching Strategy**: Service Worker caching
5. **Tree Shaking**: Remove unused code
6. **Lazy Loading**: Defer non-critical resources

## Browser Support

- Chrome 90+
- Firefox 88+
- Safari 14+
- Edge 90+

## Troubleshooting

### API Connection Failed
```
Check: VITE_API_URL in .env.local
Check: Backend is running on correct port
```

### WebSocket Connection Failed
```
Check: VITE_WEBSOCKET_URL in .env.local
Check: Backend WebSocket server running
```

### Pixel Streaming Not Working
```
Check: VITE_STREAM_URL configured correctly
Check: UE5 Pixel Streaming running on correct port
Check: Browser supports WebRTC
```

---

**Version**: 0.1.0  
**Last Updated**: 2026-05-26
