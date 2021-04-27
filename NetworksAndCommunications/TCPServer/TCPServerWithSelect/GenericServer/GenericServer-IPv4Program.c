#include <stdio.h> /* perror */
#include "TCPServer-IPv4.h"


int main(void)
{
    TCPServerResult statusCode;
    TCPServer* server = TCPServerCreate();
    if(!server)
    {
        perror("Failed to create a TCP Server...\n");
        return 1;
    }

    statusCode = TCPServerRun(server);
    if(statusCode != TCPSERVER_SUCCESS)
    {
        perror("An error has occurred while running the TCP Server...\n");
        return 1;
    }

    TCPServerDestroy(&server);

    return 0;
}