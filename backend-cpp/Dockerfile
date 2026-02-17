
FROM ubuntu:20.04 AS builder

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y \
    g++ \
    make \
    libboost-all-dev \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app

COPY . .

RUN g++ -std=c++14 -pthread -o chat_server chat-room.cpp -lboost_system

FROM ubuntu:20.04

WORKDIR /app
COPY --from=builder /app/chat_server .

EXPOSE 8080

CMD ["./chat_server", "8080"]
