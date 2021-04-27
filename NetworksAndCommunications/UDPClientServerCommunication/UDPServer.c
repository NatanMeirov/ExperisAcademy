#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h> /* ssize_t, size_t */
#include <sys/socket.h> /* standard C socket lib */
#include <errno.h>
#include <netinet/in.h> /* struct sockaddr_in */
#include <arpa/inet.h> /* inet_addr */

#define SEND_RECEIVE_LIMIT 5

int main(void)
{
    size_t i = 0;
    ssize_t sentBytes;
    ssize_t readBytes;
    unsigned int socketAddressLength;
    char buffer[4096];
    struct sockaddr_in socketAddress;
    int serverSocket = socket(AF_INET, SOCK_DGRAM, 0);

    if(serverSocket < 0)
    {
        perror("socket() failed\n");
        abort();
    }

    memset(&socketAddress, 0, sizeof(socketAddress));
    socketAddress.sin_addr.s_addr = INADDR_ANY;
    socketAddress.sin_family = AF_INET;
    socketAddress.sin_port = htons(5555);

    if(bind(serverSocket, (struct sockaddr*)(&socketAddress), sizeof(socketAddress)) < 0)
    {
        perror("bind() failed\n");
        abort();
    }

    while(i < SEND_RECEIVE_LIMIT)
    {
        socketAddressLength = sizeof(socketAddress);
        readBytes = recvfrom(serverSocket, buffer, sizeof(buffer), 0, (struct sockaddr *) &socketAddress, &socketAddressLength);
        if(readBytes < 0)
        {
            perror("recvfrom() failed\n");
            abort();
        }

        printf("Received from client: %s\n", buffer);

        sentBytes = sendto(serverSocket, "Pong", strlen("Pong"), 0, (struct sockaddr *) &socketAddress, sizeof(socketAddress));
        if(sentBytes < 0)
        {
            perror("sendto() failed\n");
            abort();
        }

        i++;
    }
}