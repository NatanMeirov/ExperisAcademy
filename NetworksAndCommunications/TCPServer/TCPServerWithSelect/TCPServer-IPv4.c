/**
 * @file TCPServer-IPv4.c
 * @author Natan Meirov (NatanMeirov@gmail.com)
 * @brief A TCP Server implementation, supports IPv4 only
 * @version 1.0
 * @date 2021-04-20
 * 
 * @copyright Copyright (c) 2021
 * 
 */


/* Includes: */

#include <asm-generic/errno.h>
#include <stddef.h> /* size_t, NULL */
#include <stdlib.h> /* malloc, free */
#include <stdio.h> /* printf, perror */
#include <string.h> /* memset */
#include <sys/select.h>
#include <unistd.h> /* fcntl */
#include <fcntl.h> /* fcntl - Special Operation on file descriptors: used to set a Not Blocking flag on sockets */
#include <sys/types.h> /* ssize_t */
#include <sys/socket.h> /* C standard socket lib */
#include <sys/select.h> /* fd_set, select */
#include <arpa/inet.h> /* htons */
#include <netinet/in.h> /* inet_addr */
#include <asm-generic/socket.h> /* SOL_SOCK, SO_REUSEADDR */
#include <errno.h> /* errno */
#include "../../../Libraries/DynamicLibraries/GenericDataStructures/Include/GenericLinkedList/GenericLinkedList.h"
#include "../../../Libraries/DynamicLibraries/GenericDataStructures/Include/GenericLinkedList/LinkedListIterator.h"
#include "../../../Libraries/DynamicLibraries/GenericDataStructures/Include/GenericLinkedList/LinkedListIteratorFunctions.h"
#include "TCPServer-IPv4.h"


/* Defines: */

#define SERVER_DEFAULT_PORT 5555
#define WAITING_CONNECTIONS_LIMIT 1020 /* Maximum hard-configured file descriptors on Linux OS: 1024 (0,1,2 - always in use, 3 will be the server's listening socket [socket is a file descriptor too]) */
#define TRUE 1
#define BUFFER_SIZE 4096 /* 4 Kb */
#define FILE_DESCRIPTORS_LIMIT 1024
#define TIMEOUT_VALUE 10

typedef int Socket;

typedef struct ServerSocket
{
    struct sockaddr_in m_serverSocketAddress;
    Socket m_listeningSocket;
} ServerSocket;

struct TCPServer
{
    ServerSocket m_serverSocket;
    LinkedList* m_waitingConnectionSocketsList;
    fd_set m_socketsSignalsIndicator;
};

typedef enum HandlingClientResult
{
    CLIENT_FINISH = 0,
    CLIENT_KEEP,
    CLIENT_ERROR
} HandlingClientResult;


/* Static Functions Declarations: */

static int InitializeTCPServer(TCPServer* _server);
static int InitializeTCPServerSocket(ServerSocket* _serverSocket);
static TCPServerResult AcceptNewClients(TCPServer* _server);
static void HandleExistingClientsRequests(TCPServer* _server, fd_set _socketsSignalsIndicator, int _clientsSocketSignalsCount);
static HandlingClientResult HandleSingleClientRequest(Socket* _clientSocket); /* Returns 0 if finished to handle the client (closed socket - remove from LinkedList), or 1 if did not finish yet (will not be removed from LinkedList) */
static void HandleSingleClientSocketClose(TCPServer* _server); /* close fd and remove from LL */
static void DestroySingleClientSocket(void* _clientSocket);
static void ShutdownTCPServerSocket(ServerSocket* _serverSocket);
static const char* MapTCPServerResultToString(TCPServerResult statusCode);
static void HandleClientMessage(char* _msg);


/* -------------------------------- Main API Functions -------------------------------------- */

TCPServer* TCPServerCreate()
{
    TCPServer* server = malloc(sizeof(TCPServer));
    if(!server)
    {
        return NULL;
    }

    if(!InitializeTCPServer(server))
    {
        if(server->m_waitingConnectionSocketsList) /* LL had initialized - destroy is required */
        {
            LinkedListDestroy(&server->m_waitingConnectionSocketsList, NULL);
        }

        free(server);

        return NULL;
    }

    return server;
}


TCPServerResult TCPServerRun(TCPServer* _server)
{
    fd_set tempSocketsSignalsIndicator;
    TCPServerResult statusCode;
    int socketsSignalsCount;

    if(!_server)
    {
        return TCPSERVER_NOT_INITIALIZED;
    }

    /* Main server loop: */
    while(TRUE)
    {
        tempSocketsSignalsIndicator = _server->m_socketsSignalsIndicator; /* First: save, because the select is deleting the fd_set's content */

        /* Block till there is an active signal from a client socket */
        if((socketsSignalsCount = select(FILE_DESCRIPTORS_LIMIT, &tempSocketsSignalsIndicator, NULL, NULL, NULL)) < 0)
        {
            perror(MapTCPServerResultToString(TCPSERVER_INTERNAL_ERROR));
            perror("select system call had failed...\n");

            return TCPSERVER_INTERNAL_ERROR;
        }

        if(FD_ISSET(_server->m_serverSocket.m_listeningSocket, &tempSocketsSignalsIndicator))
        {
            /* There is a new pending connection from a new client */
            statusCode = AcceptNewClients(_server);

            if(statusCode != TCPSERVER_SUCCESS)
            {
                perror(MapTCPServerResultToString(statusCode));

                return statusCode;
            }

            socketsSignalsCount--; /* After handling the listening socket */
        }

        if(socketsSignalsCount) /* Check if only the listening socket had a signal -> if true: can continue to be blocked by the OS using the select (so end current loop) */
        {
            HandleExistingClientsRequests(_server, tempSocketsSignalsIndicator, socketsSignalsCount);
        }
    }

    return TCPSERVER_SUCCESS;
}


void TCPServerDestroy(TCPServer **_server)
{
    if(_server && *_server)
    {
        LinkedListDestroy(&(*_server)->m_waitingConnectionSocketsList, &DestroySingleClientSocket);

        ShutdownTCPServerSocket(&((*_server)->m_serverSocket));

        free((*_server));
        (*_server) = NULL;
    }
}

/* ----------------------------- End of Main API Functions ----------------------------------- */


/* Static Functions: */

static int InitializeTCPServer(TCPServer* _server)
{
    int status;
    int optionValue = 1;

    status = InitializeTCPServerSocket(&_server->m_serverSocket);

    if(!status)
    {
        return 0;
    }

    /* Creates the LinkedList to store the accepted clients sockets */
    _server->m_waitingConnectionSocketsList = LinkedListCreate();
    if(!_server->m_waitingConnectionSocketsList)
    {
        perror(strerror(ENOMEM));
        return 0;
    }

    /* Set a reuse option for the server's port */
    if(setsockopt(_server->m_serverSocket.m_listeningSocket, SOL_SOCKET, SO_REUSEADDR, &optionValue, sizeof(optionValue)) < 0)
    {
        perror("Failed to set the server's port in reuseable mode...\n");
        return 0;
    }

    /* Bind the listening socket to the server's port */
    if(bind(_server->m_serverSocket.m_listeningSocket, (struct sockaddr*)&_server->m_serverSocket.m_serverSocketAddress, sizeof(_server->m_serverSocket.m_serverSocketAddress)) < 0)
    {
        perror("Binding the socket to the server's port had failed...\n");
        return 0;
    }

    /* Listen to the server's port - make the socket passive */
    if(listen(_server->m_serverSocket.m_listeningSocket, WAITING_CONNECTIONS_LIMIT) < 0)
    {
        perror("Failed while trying to listen to the server's port...\n");
        return 0;
    }

    FD_ZERO(&_server->m_socketsSignalsIndicator);
    FD_SET(_server->m_serverSocket.m_listeningSocket, &_server->m_socketsSignalsIndicator);

    return 1;
}


static int InitializeTCPServerSocket(ServerSocket* _serverSocket)
{
    _serverSocket->m_listeningSocket = socket(AF_INET, SOCK_STREAM, 0); /* TCP socket */
    if(_serverSocket->m_listeningSocket < 0)
    {
        perror("Failed while trying to create a socket...\n");
        return 0;
    }

    memset(&_serverSocket->m_serverSocketAddress, 0, sizeof(_serverSocket->m_serverSocketAddress)); /* Clearing the socket address object first */
    _serverSocket->m_serverSocketAddress.sin_addr.s_addr = INADDR_ANY; /* Listening to every valid ip address */
    _serverSocket->m_serverSocketAddress.sin_port = htons(SERVER_DEFAULT_PORT); /* Listening to port number 5555 */
    _serverSocket->m_serverSocketAddress.sin_family = AF_INET; /* IPv4 */

    return 1;
}


static TCPServerResult AcceptNewClients(TCPServer* _server)
{
    Socket* socketPtr = NULL;
    struct sockaddr_in clientSocketAddress;
    unsigned int clientSocketAddressSize = sizeof(clientSocketAddress);
    Socket newClientSocket = accept(_server->m_serverSocket.m_listeningSocket, (struct sockaddr*)&clientSocketAddress, &clientSocketAddressSize);

    if(newClientSocket < 0)
    {
        perror(strerror(errno));

        return TCPSERVER_SOCKET_CREATION_FAILED;
    }

    socketPtr = malloc(sizeof(Socket));
    if(!socketPtr)
    {
        close(newClientSocket);

        return TCPSERVER_MEMORY_ALLOCATION_FAILED;
    }

    *socketPtr = newClientSocket;

    if(LinkedListInsertTail(_server->m_waitingConnectionSocketsList, (void*)socketPtr) != LINKEDLIST_SUCCESS)
    {
        close(newClientSocket);
        free(socketPtr);

        return TCPSERVER_MEMORY_ALLOCATION_FAILED; /* MUST be an allocation failed error */
    }

    /* Set it to the sockets signals indicator */
    FD_SET(newClientSocket, &_server->m_socketsSignalsIndicator);

    return TCPSERVER_SUCCESS;
}


static void HandleExistingClientsRequests(TCPServer* _server, fd_set _socketsSignalsIndicator, int _clientsSocketSignalsCount)
{
    HandlingClientResult result;
    Socket* currentClientSocket = NULL;
    LinkedListIterator iterator = LinkedListIteratorBegin(_server->m_waitingConnectionSocketsList);
    LinkedListIterator endOfLinkedList = LinkedListIteratorEnd(_server->m_waitingConnectionSocketsList);

    while(iterator != endOfLinkedList)
    {
        currentClientSocket = (Socket*)LinkedListIteratorGet(iterator);

        if(FD_ISSET(*currentClientSocket, &_socketsSignalsIndicator))
        {
            /* Handle the client */
            result = HandleSingleClientRequest(currentClientSocket);
            if(result == CLIENT_FINISH || result == CLIENT_ERROR)
            {
                LinkedListIteratorRemove(iterator);
                FD_CLR(*currentClientSocket, &_server->m_socketsSignalsIndicator);
                DestroySingleClientSocket(currentClientSocket);
            }
            /* else - CLIENT_KEEP */

            _clientsSocketSignalsCount--;

            if(!_clientsSocketSignalsCount) /* Finished to handle all pending clients */
            {
                break;
            }
        }

        iterator = LinkedListIteratorNext(iterator);
    }
}


static HandlingClientResult HandleSingleClientRequest(Socket* _clientSocket)
{
    size_t i;
    ssize_t bytes;
    char buffer[BUFFER_SIZE];

    bytes = recv(*_clientSocket, buffer, BUFFER_SIZE, 0);
    if(bytes == 0)
    {
        return CLIENT_FINISH; /* The connection has finished */
    }
    else if(bytes < 0)
    {
        return CLIENT_ERROR;
    }

    HandleClientMessage(buffer);

    bytes = send(*_clientSocket, "Response from server", strlen("Response from TCP Server"), 0);
    if(bytes < 0)
    {
        return CLIENT_ERROR;
    }
    else if(bytes != strlen("Response from TCP Server"))
    {
        for(i = 0; i < TIMEOUT_VALUE; i++)
        {
            bytes = send(*_clientSocket, "Response from server", strlen("Response from TCP Server"), 0);
            if(bytes == strlen("Response from TCP Server"))
            {
                break; /* Succeed to send */
            }
        }
    }

    return CLIENT_KEEP;
}


static void ShutdownTCPServerSocket(ServerSocket* _serverSocket)
{
    close(_serverSocket->m_listeningSocket);
}


static void DestroySingleClientSocket(void* _clientSocket)
{
    close(*((Socket*) _clientSocket));
    free(_clientSocket);
}


static const char* MapTCPServerResultToString(TCPServerResult statusCode)
{
    if(statusCode == TCPSERVER_MEMORY_ALLOCATION_FAILED)
    {
        return "Failed to allocate more memory...\n";
    }
    else if(statusCode == TCPSERVER_SOCKET_CREATION_FAILED)
    {
        return "Failed while trying to create a new socket...\n";
    }
    else if(statusCode == TCPSERVER_INTERNAL_ERROR)
    {
        return "TCP Server internal error has occurred...\n";
    }
    else
    {
        return "Unknown error has occurred...\n";
    }
}


static void HandleClientMessage(char* _msg)
{
    printf("%s\n", _msg);
}
