#include "chat-room.hpp"
#include <iostream>

Session::Session(tcp::socket s, Room &room)
    : clientSocket(std::move(s)), room(room) {}

void Room::join(ParticipantPtr participant) {
    this->participants.insert(participant);
}

void Room::leave(ParticipantPtr participant) {
    this->participants.erase(participant);
}

void Room::deliver(ParticipantPtr participant, Message &message) {
    for (ParticipantPtr p : participants) {
        if (p != participant) {
            p->deliver(message);  
        }
    }
}


void Session::async_read() {
    auto self(shared_from_this());
    boost::asio::async_read_until(clientSocket , buffer , "\n" , [this , self](boost::system::error_code ec , std::size_t bytes_transferred) {
        if(!ec) {
            std::string data(boost::asio::buffers_begin(buffer.data()),
            boost::asio::buffers_begin(buffer.data()) + bytes_transferred);

            buffer.consume(bytes_transferred);
            std::cout<<"Recieved: "<< data<<std::endl;
            Message message(data);
            room.deliver(shared_from_this(), message);
            async_read();
        } else {
            room.leave(shared_from_this());
            if(ec == boost::asio::error::eof) {
                std::cout<<"Connection closed by peer"<<std::endl;
            } else {
                std::cout<<"Read error: "<<ec.message()<<std::endl;
            }
        }
    });
}

void Session::async_write() {
    auto self(shared_from_this());

    boost::asio::async_write(
        clientSocket,
        boost::asio::buffer(
            messageQueue.front().dataPtr(),
            messageQueue.front().dataLength()
        ),
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


void Session::write(Message &message) {
    messageQueue.push_back(message);

    if (messageQueue.size() == 1) {
        async_write();
    }
}

void Session::deliver(Message &message) {
    write(message);
}

void Session::start() {
    room.join(shared_from_this());
    async_read();
}

using boost::asio::ip::address_v4;

void accept_connection(boost::asio::io_context &io , char *port , tcp::acceptor &acceptor , Room &room , const tcp::endpoint &endpoint) {
    tcp::socket socket(io);
    acceptor.async_accept([&](boost::system::error_code ec , tcp::socket socket) {
        if(!ec) {
            std::shared_ptr<Session> session = std::make_shared<Session>(std::move(socket) , room);
            session->start();
        }
        accept_connection(io , port , acceptor , room , endpoint);
    });
}

int main(int argc , char *argv[]) {
    try {
        if(argc < 2) {
            std::cerr<<"Usage: server <port>";
            return 1;
        }
        Room room;
        boost::asio::io_context io;
        tcp::endpoint endpoint(tcp::v4() , atoi(argv[1]));
        tcp::acceptor acceptor(io , endpoint);
        accept_connection(io , argv[1] , acceptor , room , endpoint);
        io.run();
    } catch(std::exception &e) {
        std::cerr<<"Exception: "<<e.what()<<std::endl;
    }
    return 0;
}