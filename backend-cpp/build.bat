@echo off
echo Compiling Chat Server...
g++ -std=c++14 -I C:\boost_1_90_0 -o chat_server chat-room.cpp -lws2_32 -lmswsock
if %errorlevel% neq 0 (
    echo Compilation failed!
    exit /b %errorlevel%
)
echo Compilation successful.
exit /b 0
