#include<string>
#include<cstdlib>
#include<cstdio>
#include<cstring>

#ifndef MESSAGE_HPP
#define MESSAGE_HPP

class Message{

    public:
        enum { maxBytes = 512 };
        enum { header = 4 };

    
        Message(): bodyLength_(0){}
        
        Message(std::string message) {
            bodyLength_ = getNewBodyLength(message.size());
            encodeHeader();
            std::memcpy(data+header , message.c_str() , bodyLength_);
        }

        const char* dataPtr() const {        
            return data;
        }

        size_t dataLength() const {           
            return header + bodyLength_;
        }

        size_t getNewBodyLength(size_t newLength) {
            if(newLength > maxBytes) {
                return maxBytes;
            }
            return newLength;
        }

        std::string getData() {
            return std::string(data + header , bodyLength_);
        }

        std::string getBody() {
            return std::string(data + header, bodyLength_);
        }

        bool decodeHeader() {
            char new_header[header + 1] = "";
            strncpy(new_header , data , header);
            new_header[header] = '\0';
            int headervalue = atoi(new_header); 
            if(headervalue > maxBytes) {
                bodyLength_ = 0;
                return false;
            }
            bodyLength_ = headervalue;
            return true;
        }

        void encodeHeader() {
            char new_header[header + 1] = "";
            sprintf(new_header, "%4d" , static_cast<int>(bodyLength_));
            memcpy(data , new_header , header);
        }

    private:
        char data[header+maxBytes];
        size_t bodyLength_;
};

#endif // MESSAGE_HPP