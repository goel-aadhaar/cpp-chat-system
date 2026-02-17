#!/bin/bash

# Start C++ server in background on port 9099
./chat_server 9099 &

# Wait for it to be ready (optional, but good practice)
sleep 2

# Start Node.js Bridge on port 8080 (default)
cd bridge && node bridge.js

