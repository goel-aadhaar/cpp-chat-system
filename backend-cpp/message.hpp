#ifndef MESSAGE_HPP
#define MESSAGE_HPP

#include <string>
#include <vector>
#include <cstring>

class Message {
public:
    Message() {}
    Message(const std::string& data) : data_(data) {}
    Message(const char* data, size_t length) : data_(data, length) {}

    const char* dataPtr() const {
        return data_.c_str();
    }

    size_t dataLength() const {
        return data_.length();
    }

    std::string getData() const {
        return data_;
    }

private:
   std::string data_;
};

#endif // MESSAGE_HPP