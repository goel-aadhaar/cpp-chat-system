#include "message.hpp"
#include <iostream>
#include <boost/asio.hpp>
#include <thread>

using boost::asio::ip::tcp;

void async_read(tcp::socket &socket) {
    auto buffer = std::make_shared<boost::asio::streambuf>();
    boost::asio::async_read_until(socket , *buffer , "\n" , [&socket , buffer]
    (boost::system::error_code ec , std::size_t) {
        if(!ec) {
            std::istream is(buffer.get());
            std::string recieved;
            std::getline(is , recieved);
            if (recieved.size() >= 4) {
                recieved = recieved.substr(4);
            }
            std::cout<<"Server: "<<recieved<<std::endl;
            async_read(socket);
        }
    });
}

int main(int argc , char *argv[]) {
    if(argc < 2) {
        std::cerr<<"Provide port too as a second argument"<<std::endl;
        return 1;
    }
    boost::asio::io_context io;
    tcp::socket socket(io);
    tcp::resolver resolver(io);

    boost::asio::connect(socket , resolver.resolve("127.0.0.1" , argv[1]));
    async_read(socket);
    std::thread t([&io , &socket]() {
        while(true) {
            std::string data;
            std::cout<<"Enter message: ";
            std::getline(std::cin , data);
            data += "\n";
            boost::asio::post(io , [& , data]() {
                boost::asio::write(socket , boost::asio::buffer(data));
            });
        }
    });
    io.run();
    t.join();
    return 0;
}