/**
 * @file GenericServer-IPv4.c
 * @author Natan Meirov (NatanMeirov@gmail.com)
 * @brief A Generic TCP Server implementation, supports IPv4 only
 * @version 1.0
 * @date 2021-04-22
 * 
 * @copyright Copyright (c) 2021
 * 
 */


/* Includes: */

#include <asm-generic/errno.h>
#include <bits/types/time_t.h>
#include <stddef.h> /* size_t, NULL */
#include <stdlib.h> /* malloc, free */
#include <stdio.h> /* fprintf, FILE */
#include <string.h> /* memset */
#include <sys/select.h> /* select, fd_set and its MACROS */
#include <unistd.h>
#include <sys/types.h> /* ssize_t */
#include <sys/socket.h> /* C standard socket lib */
#include <arpa/inet.h> /* htons */
#include <netinet/in.h> /* inet_addr */
#include <asm-generic/socket.h> /* SOL_SOCK, SO_REUSEADDR */
#include <time.h> /* time */
#include "../../../../Libraries/DynamicLibraries/GenericDataStructures/Include/GenericLinkedList/GenericLinkedList.h"
#include "../../../../Libraries/DynamicLibraries/GenericDataStructures/Include/GenericLinkedList/LinkedListIterator.h"
#include "../../../../Libraries/DynamicLibraries/GenericDataStructures/Include/GenericLinkedList/LinkedListIteratorFunctions.h"
#include "GenericServer-IPv4.h"


/* Defines: */

#define SERVER_DEFAULT_PORT 5555
#define WAITING_CONNECTIONS_LIMIT 1020 /* Maximum hard-configured file descriptors on Linux OS: 1024 (0,1,2 - always in use, 3 will be the server's listening socket [socket is a file descriptor too]) */
#define TRUE 1
#define BUFFER_SIZE 4096 /* 4 Kb */
#define FILE_DESCRIPTORS_LIMIT 1024
#define TIMEOUT_VALUE 10

typedef int Socket;

typedef unsigned char Byte;

typedef struct ServerSocket
{
    struct sockaddr_in m_serverSocketAddress;
    Socket m_listeningSocket;
} ServerSocket;

struct Server
{
    ServerSocket m_serverSocket;
    LinkedList* m_waitingConnectionSocketsList;
    fd_set m_socketsSignalsIndicator;
    ClientMessageHandler m_onClientMessage;
    ErrorHandler m_onError;
    NewClientConnectionHandler m_onNewClientConnection;
    CloseClientConnectionHandler m_onCloseClientConnection;
    void* m_applicationInfo;
    FILE* m_serverLogger;
    int m_isStopServerFromRunning;
};

typedef enum HandlingClientResult
{
    CLIENT_FINISH = 0,
    CLIENT_KEEP,
    CLIENT_ERROR
} HandlingClientResult;


/* Static Functions Declarations: */

static int InitializeServer(Server* _server, int _totalAvalableClientsSockets);
static int InitializeServerSocket(ServerSocket* _serverSocket, FILE* _serverLogger);
static ServerResult AcceptNewClients(Server* _server);
static void HandleExistingClientsRequests(Server* _server, fd_set _socketsSignalsIndicator, int _clientsSocketSignalsCount);
static HandlingClientResult HandleSingleClientRequest(Socket _clientSocket, void* _messageBuffer, size_t _messageBufferSize);
static void HandleSingleClientSocketClose(Server* _server);
static void DestroySingleClientSocket(void* _clientSocket);
static void ShutdownServerSocket(ServerSocket* _serverSocket);
static const char* MapServerErrorsToMessages(ServerResult statusCode);


/* -------------------------------- Main API Functions -------------------------------------- */

Server* ServerCreate(void* _applicationInfo, ClientMessageHandler _onClientMessage, ErrorHandler _onError, NewClientConnectionHandler _onNewClientConnection, CloseClientConnectionHandler _onCloseClientConnection, int _useServerLogger)
{
    Server* server = NULL;
    int totalAvailableClientsSockets = WAITING_CONNECTIONS_LIMIT;

    if(!_onClientMessage || !_onError)
    {
        return NULL;
    }

    server = malloc(sizeof(Server));
    if(!server)
    {
        return NULL;
    }

    if(_useServerLogger)
    {
        server->m_serverLogger = fopen("GenericTCPServer.log", "a"); /* append mode */
        if(!server->m_serverLogger)
        {
            return NULL;
        }

        totalAvailableClientsSockets--; /* FILE uses a file descriptor as well */
        fprintf(server->m_serverLogger, "%ld - [INFO:] Server creation\n", time(NULL));
    }

    server->m_applicationInfo = _applicationInfo;
    server->m_onClientMessage = _onClientMessage;
    server->m_onError = _onError;
    server->m_onNewClientConnection = _onNewClientConnection;
    server->m_onCloseClientConnection = _onCloseClientConnection;

    if(!InitializeServer(server, totalAvailableClientsSockets))
    {
        /* Server initialization had failed - Freeing memory and closing open descriptors are required */

        if(server->m_waitingConnectionSocketsList) /* If LinkedLlist has initialized - destroy is required */
        {
            LinkedListDestroy(&server->m_waitingConnectionSocketsList, NULL);
        }

        if(server->m_serverLogger)
        {
            fprintf(server->m_serverLogger, "%ld - [WARNING:] Failure while trying to create the server...\n", time(NULL));
            fclose(server->m_serverLogger);
        }

        if(server->m_serverSocket.m_listeningSocket != -1) /* In that case, the server socket is initialized */
        {
            close(server->m_serverSocket.m_listeningSocket);
        }

        free(server);

        return NULL;
    }

    return server;
}


void ServerDestroy(Server **_server)
{
    if(_server && *_server)
    {
        LinkedListDestroy(&(*_server)->m_waitingConnectionSocketsList, &DestroySingleClientSocket);

        ShutdownServerSocket(&((*_server)->m_serverSocket));

        if((*_server)->m_serverLogger)
        {
            fprintf((*_server)->m_serverLogger, "%ld - [INFO:] Server shutdown\n", time(NULL));
            fclose((*_server)->m_serverLogger);
        }

        free((*_server));
        (*_server) = NULL;
    }
}

/* TODO: add a timeout mechanism to drop from the server all clients without a recent activity */
void ServerRun(Server* _server)
{
    fd_set tempSocketsSignalsIndicator;
    ServerResult statusCode;
    int socketsSignalsCount;

    if(!_server)
    {
        return; /* Cannot trigger the OnError handler - server pointer is not initialized... */
    }

    if(_server->m_serverLogger)
    {
        fprintf(_server->m_serverLogger, "%ld - [INFO:] Server runs\n", time(NULL));
    }

    /* Main server loop: */
    while(!_server->m_isStopServerFromRunning)
    {
        /* First: save the master fd_set, because the select is deleting the fd_set's content */
        tempSocketsSignalsIndicator = _server->m_socketsSignalsIndicator;

        /* Ask for a block (from the OS) till there is an active signal from a client socket */
        if((socketsSignalsCount = select(FILE_DESCRIPTORS_LIMIT, &tempSocketsSignalsIndicator, NULL, NULL, NULL)) < 0)
        {
            if(_server->m_serverLogger)
            {
                fprintf(_server->m_serverLogger, "%ld - [ERROR:] Failure while trying to be blocked by the OS using a system call...\n", time(NULL));
            }

            _server->m_onError(_server, SERVER_INTERNAL_ERROR, MapServerErrorsToMessages(SERVER_INTERNAL_ERROR), _server->m_applicationInfo);
            if(_server->m_isStopServerFromRunning) /* The application wants to stop the server? */
            {
                break; /* The main server loop will stop and the ServerRun function will finish */
            }
        }

        if(FD_ISSET(_server->m_serverSocket.m_listeningSocket, &tempSocketsSignalsIndicator))
        {
            /* There is a new pending connection from a new client */
            statusCode = AcceptNewClients(_server);

            if(statusCode != SERVER_SUCCESS)
            {
                if(_server->m_serverLogger)
                {
                    if(statusCode == SERVER_ACCEPT_CLIENT_FAILED)
                    {
                        fprintf(_server->m_serverLogger, "%ld - [ERROR:] Failure while trying to accept a new client...\n", time(NULL));
                    }
                    else if(statusCode == SERVER_MEMORY_ALLOCATION_FAILED)
                    {
                        fprintf(_server->m_serverLogger, "%ld - [ERROR:] Failure while trying allocate memory...\n", time(NULL));
                    }
                }

                _server->m_onError(_server, statusCode, MapServerErrorsToMessages(statusCode), _server->m_applicationInfo);
                if(_server->m_isStopServerFromRunning) /* The application wants to stop the server? */
                {
                    break; /* The main server loop will stop and the ServerRun function will finish */
                }
            }

            socketsSignalsCount--; /* After handling the listening socket */
        }

        if(socketsSignalsCount) /* Check if only the listening socket had a signal -> if true: can continue to be blocked by the OS using the select (so end current loop) */
        {
            HandleExistingClientsRequests(_server, tempSocketsSignalsIndicator, socketsSignalsCount);
        }
    }
}


void ServerForceStop(Server* _server)
{
    if(_server)
    {
        _server->m_isStopServerFromRunning = 1;

        if(_server->m_serverLogger)
        {
            fprintf(_server->m_serverLogger, "%ld - [WARNING:] Forced stop server from running...\n", time(NULL));
        }
    }
}


ServerResult ServerSendMessage(Server* _server, int _clientID, void* _message)
{
    /*TODO*/
    // bytes = send(_clientSocket, "Response from server", strlen("Response from TCP Server"), 0);
    // if(bytes < 0)
    // {
    //     return CLIENT_ERROR;
    // }
    // else if(bytes != strlen("Response from TCP Server"))
    // {
    //     for(i = 0; i < TIMEOUT_VALUE; i++)
    //     {
    //         bytes = send(_clientSocket, "Response from server", strlen("Response from TCP Server"), 0);
    //         if(bytes == strlen("Response from TCP Server"))
    //         {
    //             break; /* Succeed to send */
    //         }
    //     }
    // }
}


/* ----------------------------- End of Main API Functions ----------------------------------- */


/* Static Functions: */

static int InitializeServer(Server* _server, int _totalAvalableClientsSockets)
{
    int status;
    int optionValue = 1;

    status = InitializeServerSocket(&_server->m_serverSocket, _server->m_serverLogger);

    if(!status)
    {
        return 0;
    }

    /* Creates the LinkedList to store the accepted clients sockets */
    _server->m_waitingConnectionSocketsList = LinkedListCreate();
    if(!_server->m_waitingConnectionSocketsList)
    {
        if(_server->m_serverLogger)
        {
            fprintf(_server->m_serverLogger, "%ld - [ERROR:] Failure while trying to allocate memory...\n", time(NULL));
        }

        return 0;
    }

    /* Set a reuse option for the server's port */
    if(setsockopt(_server->m_serverSocket.m_listeningSocket, SOL_SOCKET, SO_REUSEADDR, &optionValue, sizeof(optionValue)) < 0)
    {
        if(_server->m_serverLogger)
        {
            fprintf(_server->m_serverLogger, "%ld - [ERROR:] Failure while trying to configure the port to be reuseable...\n", time(NULL));
        }

        return 0;
    }

    /* Bind the listening socket to the server's port */
    if(bind(_server->m_serverSocket.m_listeningSocket, (struct sockaddr*)&_server->m_serverSocket.m_serverSocketAddress, sizeof(_server->m_serverSocket.m_serverSocketAddress)) < 0)
    {
        if(_server->m_serverLogger)
        {
            fprintf(_server->m_serverLogger, "%ld - [ERROR:] Failure while trying to bind the server socket to the port...\n", time(NULL));
        }

        return 0;
    }

    /* Listen to the server's port - make the socket passive */
    if(listen(_server->m_serverSocket.m_listeningSocket, _totalAvalableClientsSockets) < 0)
    {
        if(_server->m_serverLogger)
        {
            fprintf(_server->m_serverLogger, "%ld - [ERROR:] Failure while trying to listen the port...\n", time(NULL));
        }

        return 0;
    }

    FD_ZERO(&_server->m_socketsSignalsIndicator);
    FD_SET(_server->m_serverSocket.m_listeningSocket, &_server->m_socketsSignalsIndicator);

    _server->m_isStopServerFromRunning = 0;

    if(_server->m_serverLogger)
    {
        fprintf(_server->m_serverLogger, "%ld - [INFO:] Successful server initialization\n", time(NULL));
    }

    return 1;
}


static int InitializeServerSocket(ServerSocket* _serverSocket, FILE* _serverLogger)
{
    _serverSocket->m_listeningSocket = socket(AF_INET, SOCK_STREAM, 0); /* TCP socket */
    if(_serverSocket->m_listeningSocket < 0)
    {
        if(_serverLogger)
        {
            fprintf(_serverLogger, "%ld - [ERROR:] Failure while trying to create a socket...\n", time(NULL));
        }

        return 0;
    }

    memset(&_serverSocket->m_serverSocketAddress, 0, sizeof(_serverSocket->m_serverSocketAddress)); /* Clearing the socket address object first */
    _serverSocket->m_serverSocketAddress.sin_addr.s_addr = INADDR_ANY; /* Listening to every valid ip address */
    _serverSocket->m_serverSocketAddress.sin_port = htons(SERVER_DEFAULT_PORT);
    _serverSocket->m_serverSocketAddress.sin_family = AF_INET; /* IPv4 */

    return 1;
}


static ServerResult AcceptNewClients(Server* _server)
{
    Socket* socketPtr = NULL;
    struct sockaddr_in clientSocketAddress;
    ClientInfo clientInfo;
    unsigned int clientSocketAddressSize = sizeof(clientSocketAddress);
    Socket newClientSocket = accept(_server->m_serverSocket.m_listeningSocket, (struct sockaddr*)&clientSocketAddress, &clientSocketAddressSize);

    if(newClientSocket < 0)
    {
        if(_server->m_serverLogger)
        {
            fprintf(_server->m_serverLogger, "%ld - [ERROR:] Failure while trying to accept a new client...\n", time(NULL));
        }

        return SERVER_ACCEPT_CLIENT_FAILED;
    }

    socketPtr = malloc(sizeof(Socket));
    if(!socketPtr)
    {
        close(newClientSocket);

        if(_server->m_serverLogger)
        {
            fprintf(_server->m_serverLogger, "%ld - [ERROR:] Failure while trying to allocate memory...\n", time(NULL));
        }

        return SERVER_MEMORY_ALLOCATION_FAILED;
    }

    *socketPtr = newClientSocket;

    if(LinkedListInsertTail(_server->m_waitingConnectionSocketsList, (void*)socketPtr) != LINKEDLIST_SUCCESS)
    {
        close(newClientSocket);
        free(socketPtr);

        if(_server->m_serverLogger)
        {
            fprintf(_server->m_serverLogger, "%ld - [ERROR:] Failure while trying to allocate memory...\n", time(NULL));
        }

        return SERVER_MEMORY_ALLOCATION_FAILED; /* Can ONLY be an allocation failure error */
    }

    /* Set it to the sockets signals indicator */
    FD_SET(newClientSocket, &_server->m_socketsSignalsIndicator);

    if(_server->m_serverLogger)
    {
        fprintf(_server->m_serverLogger, "%ld - [INFO:] Client %d has connected to the server\n", time(NULL), newClientSocket);
    }

    if(_server->m_onNewClientConnection) /* This function can be NULL (if not needed) */
    {
        /* Initialize the client info object */
        clientInfo.m_clientID = newClientSocket; /* Using the client's socket number (file descriptor) as an ID of the client */
        clientInfo.m_clientIPAddress = inet_ntoa(clientSocketAddress.sin_addr);
        clientInfo.m_clientPort = ntohs(clientSocketAddress.sin_port);

        _server->m_onNewClientConnection(_server, &clientInfo, _server->m_applicationInfo);
    }

    return SERVER_SUCCESS;
}


static void HandleExistingClientsRequests(Server* _server, fd_set _socketsSignalsIndicator, int _clientsSocketSignalsCount)
{
    HandlingClientResult result;
    Socket* currentClientSocket = NULL;
    Byte receivedMessageBuffer[BUFFER_SIZE];
    LinkedListIterator iterator = LinkedListIteratorBegin(_server->m_waitingConnectionSocketsList);
    LinkedListIterator endOfLinkedList = LinkedListIteratorEnd(_server->m_waitingConnectionSocketsList);

    while(iterator != endOfLinkedList)
    {
        currentClientSocket = (Socket*)LinkedListIteratorGet(iterator);

        if(FD_ISSET(*currentClientSocket, &_socketsSignalsIndicator))
        {
            /* Handle the client */
            result = HandleSingleClientRequest(*currentClientSocket, (void*)receivedMessageBuffer, BUFFER_SIZE);
            if(result == CLIENT_FINISH || result == CLIENT_ERROR)
            {
                if(_server->m_serverLogger)
                {
                    fprintf(_server->m_serverLogger, "%ld - [INFO:] Client %d has disconnected from the server\n", time(NULL), *currentClientSocket);
                }

                if(_server->m_onCloseClientConnection) /* This function can be NULL (if not needed) */
                {
                    _server->m_onCloseClientConnection(_server, *currentClientSocket, _server->m_applicationInfo);
                }

                LinkedListIteratorRemove(iterator);
                FD_CLR(*currentClientSocket, &_server->m_socketsSignalsIndicator);
                DestroySingleClientSocket(currentClientSocket);
            }
            else /* CLIENT_KEEP - message has successfully received for the client */
            {
                _server->m_onClientMessage(_server, (void*)receivedMessageBuffer, *currentClientSocket, _server->m_applicationInfo);
            }

            _clientsSocketSignalsCount--;

            if(!_clientsSocketSignalsCount) /* Finished to handle all pending clients */
            {
                break;
            }
        }

        iterator = LinkedListIteratorNext(iterator);
    }
}


static HandlingClientResult HandleSingleClientRequest(Socket _clientSocket, void* _messageBuffer, size_t _messageBufferSize)
{
    size_t i;
    ssize_t bytes;

    bytes = recv(_clientSocket, _messageBuffer, _messageBufferSize, 0);
    if(bytes == 0)
    {
        return CLIENT_FINISH; /* The connection has finished by the client */
    }
    else if(bytes < 0)
    {
        return CLIENT_ERROR; /* The connection should be finished by the server (drop the current client because of its error) */
    }

    return CLIENT_KEEP; /* The message has received */
}


static void ShutdownServerSocket(ServerSocket* _serverSocket)
{
    close(_serverSocket->m_listeningSocket);
}


static void DestroySingleClientSocket(void* _clientSocket)
{
    close(*((Socket*) _clientSocket));
    free(_clientSocket);
}


static const char* MapServerErrorsToMessages(ServerResult statusCode)
{
    if(statusCode == SERVER_MEMORY_ALLOCATION_FAILED)
    {
        return "Failed while tying to allocate memory...\n";
    }
    else if(statusCode == SERVER_ACCEPT_CLIENT_FAILED)
    {
        return "Failed while trying to accept a new client...\n";
    }
    else if(statusCode == SERVER_INTERNAL_ERROR)
    {
        return "Server internal error has occurred...\n";
    }
    else
    {
        return "Unknown error has occurred...\n";
    }
}