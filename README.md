# Async TCP Chat Server in C++ (Boost.Asio)

An **asynchronous multi-client TCP chat server and client** built using **C++ and Boost.Asio**.  
This project demonstrates how real-time messaging systems work **below WebSockets**, using raw TCP, manual message framing, and non-blocking I/O.

---

##  Features

- Asynchronous, non-blocking networking with **Boost.Asio**
- Multiple clients supported concurrently
- Broadcast messages to all connected clients except the sender
- Safe async write queue (no overlapping writes, no segfaults)
- Simple newline-based protocol (`\n`)
- Clean separation of responsibilities:
  - **Session** → one client connection
  - **Room** → broadcast hub

---

##  Architecture Overview

```
Client ──▶ Session ──▶ Room ──▶ Other Sessions ──▶ Clients
```

---

##  Project Structure

```
chat-room-cpp/
├── chat-room.hpp
├── chat-room.cpp
├── message.hpp
├── client.cpp
├── Makefile
├── .gitignore
└── README.md
```

---

##  Requirements

- C++20 compatible compiler (`g++`)
- Boost (Asio is header-only)
- Linux / WSL / MSYS2 recommended

---

##  Build

```bash
make
```

Clean build artifacts:
```bash
make clean
```

---

##  Run the Server

```bash
./chatApp 9099
```

---

##  Run Clients

```bash
./clientApp 9099
```

---

##  Message Protocol

- Messages are newline (`\n`) delimited
- Internally uses a small fixed-size header for framing
- Client strips the header before displaying messages

---

## Comparison to WebSockets

| WebSockets (JS) | This Project (C++) |
|-----------------|--------------------|
| Automatic framing | Manual framing |
| Garbage collected | Manual memory safety |
| High-level API | Low-level control |

---

## Future Improvements

- Full binary header + body protocol
- Usernames and join/leave notifications
- Thread pool support
- TLS support

---

## License

MIT
