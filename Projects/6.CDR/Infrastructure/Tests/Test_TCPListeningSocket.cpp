#include "../inc/TCPListeningSocket.hpp"
#include <string.h>
#include <iostream>


int main() {
    unsigned char* message = new unsigned char[9];
    strcpy(reinterpret_cast<char*>(message), "Response");

    nm::TCPListeningSocket socket(5678, true); // Already tested without NO BLOCKING option
    socket.Configure();
    socket.Listen();

    while(true) {
        bool hasNewConnection = socket.Accept();
        if(hasNewConnection) {
            std::cout << socket.Receive(10) << std::endl;
            socket.Send(message, 9);
        }
    }

    return 0;
}