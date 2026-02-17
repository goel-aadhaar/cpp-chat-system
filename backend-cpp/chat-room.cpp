#include "chat-room.hpp"
#include <iostream>

Session::Session(tcp::socket s, Room &room)
    : clientSocket(std::move(s)), room(room) {}

Session::~Session() {
    // Destructor
}

void Room::join(ParticipantPtr participant) {
    participants.insert(participant);
}

void Room::leave(ParticipantPtr participant) {
    participants.erase(participant);
}

void Room::deliver(ParticipantPtr participant, Message &message) {
    for (ParticipantPtr p : participants) {
        if (p != participant) {
            p->deliver(message);  
        }
    }
}

void Room::broadcast(Message &message) {
    for (ParticipantPtr p : participants) {
        p->deliver(message);
    }
}

void Session::async_read() {
    auto self(shared_from_this());
    boost::asio::async_read_until(clientSocket, buffer, "\n", 
        [this, self](boost::system::error_code ec, std::size_t bytes_transferred) {
            if(!ec) {
                std::string data(boost::asio::buffers_begin(buffer.data()),
                                 boost::asio::buffers_begin(buffer.data()) + bytes_transferred);
                buffer.consume(bytes_transferred);

                // Trim newline for processing
                std::string content = data;
                while(!content.empty() && (content.back() == '\n' || content.back() == '\r')) {
                    content.pop_back();
                }

                if (content.rfind("JOIN:", 0) == 0) {
                    // Protocol: JOIN:Username
                    username = content.substr(5);
                    std::cout << "User joined: " << username << std::endl;
                    std::string msgStr = "JOINED:" + username + "\n";
                    Message msg(msgStr);
                    room.broadcast(msg);
                } 
                else if (content.rfind("MSG:", 0) == 0) {
                    // Protocol: MSG:Content
                    if(!username.empty()) {
                        std::string chatContent = content.substr(4);
                        // Forward as: MSG:Username:Content
                        std::string msgStr = "MSG:" + username + ":" + chatContent + "\n";
                        Message msg(msgStr);
                        room.deliver(shared_from_this(), msg); // Send to others
                    }
                }
                
                async_read();
            } else {
                // Disconnected
                if(!username.empty()) {
                    std::string msgStr = "LEFT:" + username + "\n";
                    Message msg(msgStr);
                    room.broadcast(msg);
                }
                room.leave(shared_from_this());
            }
        });
}

void Session::async_write() {
    auto self(shared_from_this());
    boost::asio::async_write(
        clientSocket,
        boost::asio::buffer(messageQueue.front().dataPtr(), messageQueue.front().dataLength()),
        [this, self](boost::system::error_code ec, std::size_t) {
            if(!ec) {
                messageQueue.pop_front();
                if(!messageQueue.empty()) {
                    async_write();
                }
            } else {
                room.leave(shared_from_this());
            }
        }
    );
}

void Session::deliver(Message &message) {
    messageQueue.push_back(message);
    if (messageQueue.size() == 1) {
        async_write();
    }
}

void Session::start() {
    room.join(shared_from_this());
    async_read();
}

void accept_connection(boost::asio::io_context &io, tcp::acceptor &acceptor, Room &room) {
    acceptor.async_accept([&](boost::system::error_code ec, tcp::socket socket) {
        if(!ec) {
            std::make_shared<Session>(std::move(socket), room)->start();
        }
        accept_connection(io, acceptor, room);
    });
}

int main(int argc, char *argv[]) {
    try {
        if(argc < 2) {
            std::cerr << "Usage: server <port>\n";
            return 1;
        }
        Room room;
        boost::asio::io_context io;
        tcp::endpoint endpoint(tcp::v4(), std::atoi(argv[1]));
        tcp::acceptor acceptor(io, endpoint);
        
        std::cout << "Chat Server started on port " << argv[1] << std::endl;
        
        accept_connection(io, acceptor, room);
        io.run();
    } catch(std::exception &e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }
    return 0;
}