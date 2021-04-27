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
#include <unistd.h> /* fcntl */
#include <fcntl.h> /* fcntl - Special Operation on file descriptors: used to set a Not Blocking flag on sockets */
#include <sys/types.h> /* ssize_t */
#include <sys/socket.h> /* C standard socket lib */
#include <arpa/inet.h> /* htons */
#include <netinet/in.h> /* inet_addr */
#include <asm-generic/socket.h> /* SOL_SOCK, SO_REUSEADDR */
#include <errno.h> /* errno */
#include "../../Libraries/DynamicLibraries/GenericDataStructures/Include/GenericLinkedList/GenericLinkedList.h"
#include "../../Libraries/DynamicLibraries/GenericDataStructures/Include/GenericLinkedList/LinkedListIterator.h"
#include "../../Libraries/DynamicLibraries/GenericDataStructures/Include/GenericLinkedList/LinkedListIteratorFunctions.h"
#include "TCPServer-IPv4.h"


/* Defines: */

#define SERVER_DEFAULT_PORT 5555
#define WAITING_CONNECTIONS_LIMIT 1020 /* Maximum hard-configured file descriptors on Linux OS: 1024 (0,1,2 - always in use, 3 will be the server's listening socket [socket is a file descriptor too]) */
#define TRUE 1
#define BUFFER_SIZE 4096 /* 4 Kb */

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
static void ChangeToNoBlockingSocket(Socket* _socket);
static TCPServerResult AcceptNewClients(TCPServer* _server);
static void HandleExistingClientsRequests(TCPServer* _serve);
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
    TCPServerResult statusCode;

    if(!_server)
    {
        return TCPSERVER_NOT_INITIALIZED;
    }

    /* Main server loop: */
    while(TRUE)
    {
        statusCode = AcceptNewClients(_server);
        if(statusCode != TCPSERVER_SUCCESS)
        {
            perror(MapTCPServerResultToString(statusCode));

            return statusCode;
        }

        HandleExistingClientsRequests(_server);
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

    /* Make listening socket a NOT BLOCKING socket */
    ChangeToNoBlockingSocket(&_serverSocket->m_listeningSocket);

    memset(&_serverSocket->m_serverSocketAddress, 0, sizeof(_serverSocket->m_serverSocketAddress)); /* Clearing the socket address object first */
    _serverSocket->m_serverSocketAddress.sin_addr.s_addr = INADDR_ANY; /* Listening to every valid ip address */
    _serverSocket->m_serverSocketAddress.sin_port = htons(SERVER_DEFAULT_PORT); /* Listening to port number 5555 */
    _serverSocket->m_serverSocketAddress.sin_family = AF_INET; /* IPv4 */

    return 1;
}


static void ChangeToNoBlockingSocket(Socket* _socket)
{
    int flags;

    /* Get the current file descriptor's (here: the socket's) flags bits */
    if((flags = fcntl(*_socket, F_GETFL)) < 0) /* Error */
    {
        perror("Error on fcntl - F_GETFL: ");
        perror(strerror(errno));
        return;
    }

    /* Set the current file descriptor's (here: the socket's) flags + the No Blocking flag with "or" operations on the flags' bits */
    if(fcntl(*_socket, F_SETFL, flags | O_NONBLOCK) < 0) /* Error */
    {
        perror("Error on fcntl - F_SETFL: ");
        perror(strerror(errno));
        return;
    }

    /* O_NONBLOCK flag is enabled, so every system call will not lock (block) even while triggering a BLOCKING operation (fail with EAGAIN errno)
     * It will make the server to work in "busy waiting" mode */
}


static TCPServerResult AcceptNewClients(TCPServer* _server)
{
    Socket* socketPtr = NULL;
    struct sockaddr_in clientSocketAddress;
    unsigned int clientSocketAddressSize = sizeof(clientSocketAddress);
    Socket newClientSocket = accept(_server->m_serverSocket.m_listeningSocket, (struct sockaddr*)&clientSocketAddress, &clientSocketAddressSize);

    if(newClientSocket < 0)
    {
        if(errno != EAGAIN && errno != EWOULDBLOCK) /* A real error has occurred */
        {
            perror(strerror(errno));
            return TCPSERVER_SOCKET_CREATION_FAILED;
        }

        /* Else - just no blocking signal - continue regularly */
        return TCPSERVER_SUCCESS;
    }

    ChangeToNoBlockingSocket(&newClientSocket);

    socketPtr = malloc(sizeof(Socket));
    if(!socketPtr)
    {
        return TCPSERVER_MEMORY_ALLOCATION_FAILED;
    }

    *socketPtr = newClientSocket;

    if(LinkedListInsertTail(_server->m_waitingConnectionSocketsList, (void*)socketPtr) != LINKEDLIST_SUCCESS)
    {
        return TCPSERVER_MEMORY_ALLOCATION_FAILED; /* MUST be an allocation failed error */
    }

    return TCPSERVER_SUCCESS;
}


static void HandleExistingClientsRequests(TCPServer* _server)
{
    HandlingClientResult result;
    Socket* currentClientSocket = NULL;
    LinkedListIterator iterator = LinkedListIteratorBegin(_server->m_waitingConnectionSocketsList);
    LinkedListIterator endOfLinkedList = LinkedListIteratorEnd(_server->m_waitingConnectionSocketsList);

    while(iterator != endOfLinkedList)
    {
        currentClientSocket = (Socket*)LinkedListIteratorGet(iterator);

        result = HandleSingleClientRequest(currentClientSocket);
        if(result == CLIENT_FINISH || result == CLIENT_ERROR)
        {
            LinkedListIteratorRemove(iterator);
            DestroySingleClientSocket(currentClientSocket);
        }
        /* else - CLIENT_KEEP */

        iterator = LinkedListIteratorNext(iterator);
    }
}


static HandlingClientResult HandleSingleClientRequest(Socket* _clientSocket)
{
    ssize_t bytes;
    char buffer[BUFFER_SIZE];

    bytes = recv(*_clientSocket, buffer, BUFFER_SIZE, 0);
    if(bytes == 0)
    {
        return CLIENT_FINISH; /* The connection has finished */
    }
    else if(bytes < 0)
    {
        if(errno != EAGAIN && errno != EWOULDBLOCK) /* A real error has occurred */
        {
            return CLIENT_ERROR;
        }

        return CLIENT_KEEP; /* Just no block */
    }

    HandleClientMessage(buffer);

    bytes = send(*_clientSocket, "Response from server", strlen("Response from TCP Server"), 0);
    if(bytes < 0) /* TODO: check if all bytes are received */
    {
        if(errno != EAGAIN && errno != EWOULDBLOCK) /* A real error has occurred */
        {
            return CLIENT_ERROR;
        }

        return CLIENT_KEEP; /* Just no block */
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
    else
    {
        return "TCP Server internal error has occurred...\n";
    }
}


static void HandleClientMessage(char* _msg)
{
    printf("%s\n", _msg);
}