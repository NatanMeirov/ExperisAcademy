// A temporary executable to test the providers send and to supply cdr files to the processor

#include <iostream>
#include "inc/Provider.hpp"


#define LOOPBACK "127.0.0.1"


int main() {
    std::cout << "Press any key to send the files ";
    char unUsed;
    std::cin >> unUsed;

    nm::cdr::Provider provider(LOOPBACK, 4040);
    provider.SendCdrFiles();

    // TODO: send files every 5 minutes (loop and sleep after last send operation)

    return 0;
}