# Backend Setup Guide

## Overview

The Soccer-Game backend is a Node.js/Express microservice architecture providing:
- RESTful API for game operations
- Real-time WebSocket communication
- Authentication & authorization
- Player management
- Match orchestration
- Analytics & statistics

## Quick Start

### Prerequisites
- Node.js 18+ (LTS recommended)
- npm 9+
- MongoDB 6.0+
- Git

### Installation

```bash
cd Backend
npm install
```

### Environment Variables

Create `.env` file:
```env
# Server Configuration
NODE_ENV=development
PORT=3001
HOST=localhost

# Database
MONGODB_URI=mongodb://localhost:27017/soccer-game
DB_NAME=soccer-game

# JWT Authentication
JWT_SECRET=your-super-secret-key-change-in-production
JWT_EXPIRY=24h
JWT_REFRESH_EXPIRY=7d

# Pixel Streaming
PIXEL_STREAM_URL=http://localhost:8015
PIXEL_STREAM_PORT=8015

# CORS
CORS_ORIGIN=http://localhost:3000

# Logging
LOG_LEVEL=debug

# Email (Optional)
SMTP_HOST=smtp.gmail.com
SMTP_PORT=587
SMTP_USER=your-email@example.com
SMTP_PASS=your-password
```

### Running Development

```bash
# Development with auto-reload
npm run dev

# Production
npm run build
npm start

# Running tests
npm test
npm test:watch

# Code linting
npm run lint

# Format code
npm run format
```

## Directory Structure

```
Backend/
├── src/
│   ├── index.js              # Entry point
│   ├── config/
│   │   ├── database.js       # MongoDB connection
│   │   ├── environment.js    # Environment variables
│   │   └── logger.js         # Logging setup
│   ├── middleware/
│   │   ├── auth.js           # JWT authentication
│   │   ├── errorHandler.js   # Error handling
│   │   └── validation.js     # Input validation
│   ├── routes/
│   │   ├── auth.js           # Authentication endpoints
│   │   ├── players.js        # Player management
│   │   ├── matches.js        # Match operations
│   │   └── statistics.js     # Analytics
│   ├── controllers/
│   │   ├── authController.js
│   │   ├── playerController.js
│   │   ├── matchController.js
│   │   └── statsController.js
│   ├── models/
│   │   ├── User.js           # User schema
│   │   ├── Player.js         # Player profile
│   │   ├── Match.js          # Match record
│   │   └── Statistics.js     # Player stats
│   ├── services/
│   │   ├── authService.js
│   │   ├── playerService.js
│   │   ├── matchService.js
│   │   ├── streamingService.js
│   │   └── notificationService.js
│   └── utils/
│       ├── validators.js
│       ├── helpers.js
│       └── constants.js
├── tests/
│   ├── unit/
│   └── integration/
├── .env                      # Environment variables (local)
├── .env.example              # Environment template
├── package.json
└── README.md
```

## API Endpoints

### Authentication
```
POST   /api/v1/auth/register      - Register new user
POST   /api/v1/auth/login         - Login user
POST   /api/v1/auth/refresh       - Refresh token
POST   /api/v1/auth/logout        - Logout user
GET    /api/v1/auth/me            - Get current user
```

### Players
```
GET    /api/v1/players            - List all players
GET    /api/v1/players/:id        - Get player profile
PUT    /api/v1/players/:id        - Update profile
GET    /api/v1/players/:id/stats  - Get statistics
GET    /api/v1/leaderboard        - Global leaderboard
```

### Matches
```
POST   /api/v1/matches            - Create match
GET    /api/v1/matches            - List matches
GET    /api/v1/matches/:id        - Get match details
PUT    /api/v1/matches/:id        - Update match
DELETE /api/v1/matches/:id        - Delete match
GET    /api/v1/matches/:id/stream - Get streaming URL
POST   /api/v1/matches/:id/join   - Join match
POST   /api/v1/matches/:id/leave  - Leave match
```

## WebSocket Events

### Connection Events
```javascript
io.on('connection', (socket) => {
  socket.on('player:join', (data) => {
    // Handle player joining
  });
  
  socket.on('match:state', (data) => {
    // Handle match state update
  });
  
  socket.on('disconnect', () => {
    // Handle player disconnect
  });
});
```

## Database Models

### User Schema
```javascript
{
  _id: ObjectId,
  username: String,
  email: String,
  password: String (hashed),
  createdAt: Date,
  updatedAt: Date
}
```

### Player Schema
```javascript
{
  _id: ObjectId,
  userId: ObjectId,
  name: String,
  position: String,
  stats: {
    goals: Number,
    assists: Number,
    matches: Number,
    wins: Number
  },
  level: Number,
  experience: Number
}
```

### Match Schema
```javascript
{
  _id: ObjectId,
  homeTeam: [ObjectId],
  awayTeam: [ObjectId],
  score: { home: Number, away: Number },
  status: String,
  startedAt: Date,
  endedAt: Date,
  streamUrl: String
}
```

## Testing

```bash
# Run all tests
npm test

# Watch mode
npm test:watch

# Coverage report
npm test -- --coverage

# Run specific test
npm test -- authService.test.js
```

## Deployment

### Docker

```bash
# Build image
docker build -t soccer-game-backend .

# Run container
docker run -p 3001:3001 --env-file .env soccer-game-backend
```

### Environment Variables (Production)
- Change `NODE_ENV` to `production`
- Use strong `JWT_SECRET`
- Configure production MongoDB URI
- Enable HTTPS
- Setup rate limiting

### Health Check

```bash
curl http://localhost:3001/api/health
```

Expected response:
```json
{
  "status": "healthy",
  "timestamp": "2026-05-26T10:00:00Z",
  "version": "0.1.0"
}
```

## Monitoring & Logging

- **Logger**: Pino (structured JSON logging)
- **Error Tracking**: Console logging in development
- **Performance Monitoring**: Response time logging
- **Security**: Authentication attempt logging

View logs:
```bash
# Real-time logs
npm run dev 2>&1 | grep -v node_modules

# Pretty logs (development)
LOG_LEVEL=debug npm run dev
```

## Security Best Practices

1. **Keep dependencies updated**: `npm audit fix`
2. **Use environment variables**: Never hardcode secrets
3. **Validate input**: Use express-validator
4. **Rate limiting**: Implement per user/IP
5. **CORS**: Restrict to allowed origins
6. **HTTPS**: Always use in production
7. **Password hashing**: Bcrypt with salt rounds 12+
8. **JWT rotation**: Implement refresh token strategy

## Troubleshooting

### MongoDB Connection Failed
```bash
# Check MongoDB is running
mongod --version

# Verify connection URI in .env
MONGODB_URI=mongodb://localhost:27017/soccer-game
```

### Port Already in Use
```bash
# Find process using port 3001
lsof -i :3001

# Kill process
kill -9 <PID>

# Or change PORT in .env
PORT=3002
```

### JWT Token Expired
```
Error: jwt expired
Solution: Use refresh token endpoint or login again
```

## Performance Optimization

1. **Database Indexing**: Index frequently queried fields
2. **Caching**: Implement Redis for session data
3. **API Response Compression**: Gzip enabled
4. **Connection Pooling**: MongoDB connection pool
5. **Async Operations**: Use async/await
6. **Error Handling**: Proper error responses

---

**Version**: 0.1.0  
**Last Updated**: 2026-05-26
