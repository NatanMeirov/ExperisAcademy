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
    int clientSocket = socket(AF_INET, SOCK_DGRAM, 0);

    if(clientSocket < 0)
    {
        perror("socket() failed\n");
        abort();
    }

    memset(&socketAddress, 0, sizeof(socketAddress));
    socketAddress.sin_addr.s_addr = inet_addr("127.0.0.1");
    socketAddress.sin_family = AF_INET;
    socketAddress.sin_port = htons(5555);

    while(i < SEND_RECEIVE_LIMIT)
    {
        sentBytes = sendto(clientSocket, "Ping", strlen("Ping"), 0, (struct sockaddr *) &socketAddress, sizeof(socketAddress));
        if(sentBytes < 0)
        {
            perror("sendto() failed\n");
            abort();
        }

        socketAddressLength = sizeof(socketAddress);
        readBytes = recvfrom(clientSocket, buffer, sizeof(buffer), 0, (struct sockaddr *) &socketAddress, &socketAddressLength);
        if(readBytes < 0)
        {
            perror("recvfrom() failed\n");
            abort();
        }

        printf("Received from server: %s\n", buffer);

        i++;
    }
}