#include <stdio.h> /* printf */
#include <stdlib.h> /* rand */
#include <time.h> /* time */
#include <string.h> /* memset */
#include <unistd.h> /* close */
#include <sys/types.h> /* ssize_t */
#include <sys/socket.h> /* socket */
#include <netinet/in.h>
#include <arpa/inet.h>

#define CLIENTS_NUMBER 10
#define SERVER_DEFAULT_PORT 5555
#define TRUE 1
#define DISCONNECTED 0
#define CREATE_CONNECTION_PERCENTAGE 30
#define CLOSE_CONNECTION_PERCENTAGE 5
#define COMMUNICATE_WITH_SERVER_PERCENTAGE 30 /* Currently - 25% (from 5% to 30%) */
#define BUFFER_LENGTH 800

typedef int Socket;

static void RunMultiClientGenerator(void);
static Socket CreateNewConnectionToServer(struct sockaddr_in* _serverAddress);
static void CloseExistingConnectionToServer(Socket _socketToDisconnect);
static Socket CommunicateWithServer(Socket _socket, struct sockaddr_in* _serverAddress);


int main(void)
{
    RunMultiClientGenerator();

    return 0;
}


static void RunMultiClientGenerator(void)
{
    Socket clientsArray[CLIENTS_NUMBER] = {0};
    struct sockaddr_in serverAddress;
    size_t currentClient = 0;
    int percentage;

    memset(&serverAddress, 0, sizeof(serverAddress));
    serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1"); /* loopback (localhost) */
    serverAddress.sin_port = htons(SERVER_DEFAULT_PORT);
    serverAddress.sin_family = AF_INET; /* IPv4 */

    while(TRUE)
    {
        srand(time(NULL));
        percentage = (rand() % 100) + 1; /* From 1% to 100% */

        if(clientsArray[currentClient] == DISCONNECTED)
        {
            if(percentage <= CREATE_CONNECTION_PERCENTAGE)
            {
                clientsArray[currentClient] = CreateNewConnectionToServer(&serverAddress);
            }
        }
        else /* client is connected already */
        {
            if(percentage <= CLOSE_CONNECTION_PERCENTAGE)
            {
                CloseExistingConnectionToServer(clientsArray[currentClient]);
                clientsArray[currentClient] = 0; /* store 0 in clientArray[curr] (== disconnected) */
            }
            else if(percentage < COMMUNICATE_WITH_SERVER_PERCENTAGE)
            {
                clientsArray[currentClient] = CommunicateWithServer(clientsArray[currentClient], &serverAddress);
            }
        }

        currentClient = (currentClient + 1) % CLIENTS_NUMBER;
    }
}


static Socket CreateNewConnectionToServer(struct sockaddr_in* _serverAddress)
{
    Socket clientSocket = socket(AF_INET, SOCK_STREAM, 0); /* TCP connection */
    if(clientSocket < 0)
    {
        /* Failed to create a socket - return 0 to the clientArray (== disconnected) */
        perror("Failed to create socket...\n");
        sleep(3);

        return 0;
    }

    if(connect(clientSocket, (struct sockaddr*)_serverAddress, sizeof(*_serverAddress)) < 0)
    {
        /* Failed to connect - close socket and return 0 to the clientArray (== disconnected) */
        perror("Failed to connect...\n");
        sleep(3);

        close(clientSocket);
    }

    return clientSocket;
}


static void CloseExistingConnectionToServer(Socket _socketToDisconnect)
{
    close(_socketToDisconnect);
}


static Socket CommunicateWithServer(Socket _socket, struct sockaddr_in* _serverAddress)
{
    unsigned int bytes;
    char* buffer = malloc(BUFFER_LENGTH);
    if(!buffer)
    {
        return 0;
    }

    sprintf(buffer, "Request from client %d", _socket);

    bytes = send(_socket, buffer, strlen(buffer), 0);
    if(bytes < 0)
    {
        perror("Failed on send to server...\n");
        sleep(3);

        return 0;
    }
    else /* Succeed to send - waiting for response */
    {
        bytes = recv(_socket, buffer, BUFFER_LENGTH, 0);
        if(bytes < 0)
        {
            perror("Failed on receive from server...\n");
            sleep(3);

            return 0;
        }
        else if(bytes == 0)
        {
            CloseExistingConnectionToServer(_socket);
            return 0; /* Set 0 in the array (== disconnect) */
        }
        else /* Succeed to get a response from the server */
        {
            printf("%s\n", buffer);
        }
    }

    free(buffer);

    return _socket; /* Keep the socket in the array */
}