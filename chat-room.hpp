#ifndef CHATROOM_HPP
#define CHATROOM_HPP

#include<iostream>
#include <set>
#include <memory>
#include<deque>
#include <boost/asio.hpp>
#include "message.hpp"

using boost::asio::ip::tcp;

class Participant {
    public:
        virtual void deliver(Message &message) = 0;
        virtual void write(Message &message) = 0;
        virtual ~Participant() = default;
};

typedef std::shared_ptr<Participant> ParticipantPtr;

class Room {
    public:
        void join(ParticipantPtr participant);
        void leave(ParticipantPtr participant);
        void deliver(ParticipantPtr participant , Message  &message);

    private:
        std::deque<Message> messageQueue;
        enum {MaxParticipants = 100};
        std::set<ParticipantPtr> participants;
};

class Session: public Participant, public std::enable_shared_from_this<Session> {
    public:
        Session(tcp::socket s , Room &room);

        void start();
        void deliver(Message &message) override;
        void write(Message &message) override;
        void async_read();
        void async_write();
    
    private:
        tcp::socket clientSocket;
        boost::asio::streambuf buffer;
        Room &room;
        std::deque<Message> messageQueue;
};



#endif // CHATROOM_HPP