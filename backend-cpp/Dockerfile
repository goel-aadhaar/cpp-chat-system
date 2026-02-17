# Build stage for C++
FROM ubuntu:20.04 AS builder

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y \
    g++ \
    make \
    libboost-all-dev \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app
COPY . .

# Build C++ server
RUN g++ -std=c++14 -pthread -o chat_server chat-room.cpp -lboost_system

# Run stage
FROM ubuntu:20.04

# Install runtime dependencies + Node.js
RUN apt-get update && apt-get install -y \
    libboost-system-dev \
    curl \
    && curl -fsSL https://deb.nodesource.com/setup_18.x | bash - \
    && apt-get install -y nodejs \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app

# Copy C++ executable
COPY --from=builder /app/chat_server .

# Copy Bridge Server files (assuming they are in ../bridge-server relative to Dockerfile, 
# but Docker build context is usually the root if setup correctly. 
# However, user's previous context was backend-cpp. 
# I will assume the context is the REPO ROOT to make this clean, 
# OR I will just copy the files if I can.
# Use caution: if context is backend-cpp, I can't access ../bridge-server.
# I will instructing the user to build from ROOT context.)

# Let's assume we copy bridge files manually or they are passed in context.
# To be safe, I'll inline the bridge code if I have to, OR assume root context.
# Given the user's previous "Root Directory" setting on Render was 'backend-cpp', 
# Render only sees that folder.
# WE NEED TO TELL RENDER TO USE ROOT DIRECTORY '.' AND BUILD CONTEXT '.'
# OR move bridge-server INTO backend-cpp.

# FASTEST FIX: Create bridge.js INSIDE the container or copy it if it's there.
# Since I can't easily move files on the user's local via Dockerfile alone...
# I will WRITRE the bridge.js content into the Dockerfile or start.sh? 
# No, that's messy.

# BETTER: Ask user to move bridge-server or change Render config.
# Actually, I can just write the bridge.js file in the Dockerfile if it's small.
# It is 46 lines. That's acceptable for a robust "fix it" solution.

COPY start.sh .
RUN chmod +x start.sh

# Create bridge directory
WORKDIR /app/bridge
# We will init a package.json and install ws
RUN npm init -y && npm install ws

# Write bridge.js directly here to avoid context issues
RUN echo 'const WebSocket = require("ws"); \
const net = require("net"); \
const wss = new WebSocket.Server({ port: 8080 }); \
wss.on("connection", (ws) => { \
    console.log("Browser connected"); \
    const tcpClient = net.createConnection({ host: "127.0.0.1", port: 9099 }, () => console.log("Connected to C++ server")); \
    tcpClient.on("error", (err) => { console.error("TCP Client Error:", err.message); ws.close(); }); \
    tcpClient.on("close", () => { ws.close(); }); \
    ws.on("message", (msg) => { if (!tcpClient.destroyed) tcpClient.write(msg + "\\n"); }); \
    tcpClient.on("data", (data) => { if (ws.readyState === WebSocket.OPEN) ws.send(data.toString()); }); \
    ws.on("close", () => { if (!tcpClient.destroyed) tcpClient.end(); }); \
});' > bridge.js

WORKDIR /app

EXPOSE 8080

CMD ["./start.sh"]
