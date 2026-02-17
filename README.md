# Chat-Room Application (C++ & WebSocket)

A high-performance chat application built from scratch to demonstrate low-level network programming with C++. It bridges a raw TCP C++ backend with a modern Web Frontend using a Node.js proxy.


##  Features
- **C++ Backend**: Asynchronous TCP server using `Boost.Asio` for high-concurrency handling.
- **Node.js Bridge**: Translates WebSocket traffic (Browser) to raw TCP (C++ Server).
- **Modern UI**: "Fair Dark Theme" frontend with real-time message updates.
- **Usernames**: Custom username support with system notifications (Join/Leave).
- **Protocol**: Custom text-based protocol (`JOIN`, `MSG`, `LEFT`).

## Architecture
```mermaid
graph LR
    A[Web Client<br>(Browser)] -- WebSocket --> B[Node.js Bridge]
    B -- Raw TCP --> C[C++ Backend]
    C -- Raw TCP --> B
    B -- WebSocket --> A
```

## Prerequisites
- **C++ Compiler**: MinGW (g++) or similar supporting C++14.
- **Boost Libraries**: Required for `asio`. (Project configured for `C:\boost_1_90_0`).
- **Node.js**: For the bridge server.

## Build & Run

### 1. Backend (C++)
Navigate to `backend-cpp` and compile:
```cmd
cd backend-cpp
build
```
*(`build.bat` wraps the g++ command: `g++ -std=c++14 -I C:\boost_1_90_0 ...`)*

Run the server:
```cmd
chat_server 9099
```

### 2. Bridge Server (Node.js)
Navigate to `bridge-server`:
```cmd
cd bridge-server
node bridge.js
```

### 3. Frontend
Open `web-client/index.html` in your browser. Enter a username and start chatting!

## Protocol
- **JOIN**: `JOIN:Username` (Client -> Server)
- **MSG**: `MSG:Content` (Client -> Server) -> `MSG:Username:Content` (Broadcast)
- **LEFT**: `LEFT:Username` (Server Broadcast on disconnect)

## License
MIT
.md
