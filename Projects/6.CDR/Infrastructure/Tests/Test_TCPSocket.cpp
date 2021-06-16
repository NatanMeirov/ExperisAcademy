#include "../inc/TCPSocket.hpp"
#include <string.h>
#include <iostream>


int main() {
    unsigned char* message = new unsigned char[7];
    strcpy(reinterpret_cast<char*>(message), "Hello!");

    nm::TCPSocket socket("127.0.0.1", 5678);
    socket.Connect();
    socket.Send(message, 7);
    std::cout << socket.Receive(10) << std::endl;

    return 0;
}