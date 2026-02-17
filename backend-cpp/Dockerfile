# Build stage
FROM ubuntu:20.04 AS builder

# Avoid prompts from apt
ENV DEBIAN_FRONTEND=noninteractive

# Install build dependencies
RUN apt-get update && apt-get install -y \
    g++ \
    make \
    libboost-all-dev \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app

# Copy ALL source files
COPY . .

# Build the application
# Explicitly link pthread and boost_system
RUN g++ -std=c++14 -pthread -o chat_server chat-room.cpp -lboost_system

# Run stage
FROM ubuntu:20.04

# Install runtime dependencies (boost libs)
RUN apt-get update && apt-get install -y \
    libboost-system-dev \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app

# Copy the executable from the builder stage
COPY --from=builder /app/chat_server .

# Expose the port
EXPOSE 8080

# Run the server
CMD ["./chat_server", "8080"]
