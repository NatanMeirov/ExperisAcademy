/**
 * @file GenericServer-IPv4.c
 * @author Natan Meirov (NatanMeirov@gmail.com)
 * @brief A Generic TCP Server implementation, supports IPv4 only
 * In Version 2: improved performance of the server driver
 * @version 2.2
 * @date 2021-06-15
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
#include "GenericLinkedList.h"
#include "LinkedListIterator.h"
#include "LinkedListIteratorFunctions.h"
#include "GenericServer-IPv4.h"


/* Defines: */

#define TRUE 1
#define FILE_DESCRIPTORS_LIMIT 1024
#define TIMEOUT_VALUE 10
#define MIN_BUFFER_SIZE 1024
#define MIN_PORT_VALUE 1025
#define MAX_PORT_VALUE 64000
#define MAX_CONNECTED_CLIENTS_AT_SAME_TIME_ON_SERVER 1020

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
    size_t m_maxWaitingConnections;
    fd_set m_socketsSignalsIndicator;
    ClientMessageHandler m_onClientMessage;
    ErrorHandler m_onError;
    NewClientConnectionHandler m_onNewClientConnection;
    CloseClientConnectionHandler m_onCloseClientConnection;
    void* m_applicationInfo; /* Application Context */
    FILE* m_serverLogger;
    int m_isStopServerFromRunning;
    unsigned int m_currentConnectedClientsCount;
    unsigned int m_maxAmountOfConnectedClientsAtSameTime;
    size_t m_receivedMessagesBufferSize;
    Byte* m_receivedMessagesBuffer;
};

typedef enum HandlingClientResult
{
    CLIENT_FINISH = 0,
    CLIENT_KEEP,
    CLIENT_ERROR
} HandlingClientResult;


/* Static Functions Declarations: */

static int InitializeServer(Server* _server, int _totalAvalableClientsSockets, unsigned int _serverPort);
static int InitializeServerSocket(ServerSocket* _serverSocket, FILE* _serverLogger, unsigned int _serverPort);
static ServerResult AcceptNewClients(Server* _server);
static void HandleExistingClientsRequests(Server* _server, fd_set _socketsSignalsIndicator, int _clientsSocketSignalsCount);
static HandlingClientResult HandleSingleClientRequest(Socket _clientSocket, void* _messageBuffer, size_t _messageBufferSize);
static void DisconnectAndRemoveClientFromServer(Server* _server, void* _elementToRemove, Socket _clientSocket);
static HandlingClientResult HandleResponse(Response* _response);
static void DestroySingleClientSocket(void* _clientSocket);
static void ShutdownServerSocket(ServerSocket* _serverSocket);
static const char* MapServerErrorsToMessages(ServerResult _statusCode);


/* -------------------------------- Main API Functions -------------------------------------- */

Server* ServerCreate(void* _applicationInfo, ClientMessageHandler _onClientMessage, ErrorHandler _onError, NewClientConnectionHandler _onNewClientConnection, CloseClientConnectionHandler _onCloseClientConnection, unsigned int _serverPort, size_t _maxWaitingConnections, size_t _receivedMessagesBufferSize)
{
    Server* server = NULL;

    if(!_onClientMessage || !_onError)
    {
        return NULL;
    }

    if(_serverPort < MIN_PORT_VALUE || _serverPort > MAX_PORT_VALUE || _maxWaitingConnections == 0 || _receivedMessagesBufferSize < MIN_BUFFER_SIZE)
    {
        return NULL;
    }

    server = (Server*)malloc(sizeof(Server));
    if(!server)
    {
        return NULL;
    }

    server->m_maxWaitingConnections = _maxWaitingConnections;
    server->m_maxAmountOfConnectedClientsAtSameTime = MAX_CONNECTED_CLIENTS_AT_SAME_TIME_ON_SERVER;

    #ifdef USE_SERVER_LOGGER

    server->m_serverLogger = fopen("GenericTCPServer.log", "a"); /* append mode */
    if(!server->m_serverLogger)
    {
        return NULL;
    }

    --(server->m_maxAmountOfConnectedClientsAtSameTime); /* FILE uses a file descriptor as well */
    fprintf(server->m_serverLogger, "%ld - [INFO:] Server creation\n", time(NULL));

    #endif /* #ifdef USE_SERVER_LOGGER */

    server->m_applicationInfo = _applicationInfo;
    server->m_receivedMessagesBufferSize = _receivedMessagesBufferSize;
    server->m_onClientMessage = _onClientMessage;
    server->m_onError = _onError;
    server->m_onNewClientConnection = _onNewClientConnection;
    server->m_onCloseClientConnection = _onCloseClientConnection;

    if(!InitializeServer(server, server->m_maxWaitingConnections, _serverPort))
    {
        /* Server initialization had failed - Freeing memory and closing open descriptors are required */

        if(server->m_waitingConnectionSocketsList) /* If LinkedLlist has initialized - destroy is required */
        {
            LinkedListDestroy(&server->m_waitingConnectionSocketsList, NULL);
        }

        if(server->m_receivedMessagesBuffer)
        {
            free(server->m_receivedMessagesBuffer);
        }

        if(server->m_serverSocket.m_listeningSocket != -1) /* In that case, the server socket is initialized */
        {
            close(server->m_serverSocket.m_listeningSocket);
        }

        #ifdef USE_SERVER_LOGGER

        fprintf(server->m_serverLogger, "%ld - [WARNING:] Failure while trying to create the server...\n", time(NULL));
        fclose(server->m_serverLogger);

        #endif /* #ifdef USE_SERVER_LOGGER */

        free(server);

        return NULL;
    }

    return server;
}


void ServerDestroy(Server** _server)
{
    if(_server && *_server)
    {
        LinkedListDestroy(&(*_server)->m_waitingConnectionSocketsList, &DestroySingleClientSocket);
        free((*_server)->m_receivedMessagesBuffer);
        ShutdownServerSocket(&((*_server)->m_serverSocket));

        #ifdef USE_SERVER_LOGGER

        fprintf((*_server)->m_serverLogger, "%ld - [INFO:] Server shutdown\n", time(NULL));
        fclose((*_server)->m_serverLogger);

        #endif /* #ifdef USE_SERVER_LOGGER */

        free((*_server));
        *_server = NULL;
    }
}

/* TODO: maybe add in the future a timeout mechanism to drop from the server all clients without a recent activity */
void ServerRun(Server* _server)
{
    fd_set tempSocketsSignalsIndicator;
    ServerResult statusCode;
    int socketsSignalsCount;

    if(!_server)
    {
        return; /* Cannot trigger the OnError handler - server pointer is not initialized... */
    }

    #ifdef USE_SERVER_LOGGER

    fprintf(_server->m_serverLogger, "%ld - [INFO:] Server runs\n", time(NULL));

    #endif /* #ifdef USE_SERVER_LOGGER */

    /* Main server loop: */
    while(!_server->m_isStopServerFromRunning)
    {
        /* First: save the master fd_set, because the select is deleting the fd_set's content */
        tempSocketsSignalsIndicator = _server->m_socketsSignalsIndicator;

        /* Ask for a block (from the OS) till there is an active signal from a client socket */
        if((socketsSignalsCount = select(FILE_DESCRIPTORS_LIMIT, &tempSocketsSignalsIndicator, NULL, NULL, NULL)) < 0)
        {
            #ifdef USE_SERVER_LOGGER

            fprintf(_server->m_serverLogger, "%ld - [ERROR:] Failure while trying to be blocked by the OS using a system call...\n", time(NULL));

            #endif /* #ifdef USE_SERVER_LOGGER */

            _server->m_isStopServerFromRunning = _server->m_onError(SERVER_INTERNAL_ERROR, MapServerErrorsToMessages(SERVER_INTERNAL_ERROR), _server->m_applicationInfo);
            if(_server->m_isStopServerFromRunning)
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
                #ifdef USE_SERVER_LOGGER

                if(statusCode == SERVER_ACCEPT_CLIENT_FAILED)
                {
                    fprintf(_server->m_serverLogger, "%ld - [ERROR:] Failure while trying to accept a new client...\n", time(NULL));
                }
                else if(statusCode == SERVER_MEMORY_ALLOCATION_FAILED)
                {
                    fprintf(_server->m_serverLogger, "%ld - [ERROR:] Failure while trying to allocate memory...\n", time(NULL));
                }

                #endif /* #ifdef USE_SERVER_LOGGER */

                _server->m_isStopServerFromRunning = _server->m_onError(statusCode, MapServerErrorsToMessages(statusCode), _server->m_applicationInfo);
                if(_server->m_isStopServerFromRunning)
                {
                    break; /* The main server loop will stop and the ServerRun function will finish */
                }
            }

            --socketsSignalsCount; /* After handling the listening socket */
        }

        if(socketsSignalsCount) /* Check if only the listening socket had a signal -> if true: can continue to be blocked by the OS using the select (so end current loop) */
        {
            HandleExistingClientsRequests(_server, tempSocketsSignalsIndicator, socketsSignalsCount);
        }
    }
}


/* ----------------------------- End of Main API Functions ----------------------------------- */


/* Static Functions: */

static int InitializeServer(Server* _server, int _totalAvalableClientsSockets, unsigned int _serverPort)
{
    int status;
    int optionValue = 1;

    status = InitializeServerSocket(&_server->m_serverSocket, _server->m_serverLogger, _serverPort);

    if(!status)
    {
        return 0;
    }

    /* Creates the LinkedList to store the accepted clients sockets */
    _server->m_waitingConnectionSocketsList = LinkedListCreate();
    if(!_server->m_waitingConnectionSocketsList)
    {
        #ifdef USE_SERVER_LOGGER

        fprintf(_server->m_serverLogger, "%ld - [ERROR:] Failure while trying to allocate memory...\n", time(NULL));

        #endif /* #ifdef USE_SERVER_LOGGER */

        return 0;
    }

    _server->m_receivedMessagesBuffer = (Byte*)malloc(_server->m_receivedMessagesBufferSize);
    if(!_server->m_receivedMessagesBuffer)
    {
        #ifdef USE_SERVER_LOGGER

        fprintf(_server->m_serverLogger, "%ld - [ERROR:] Failure while trying to allocate memory...\n", time(NULL));

        #endif /* #ifdef USE_SERVER_LOGGER */

        return 0;
    }

    /* Set a reuse option for the server's port */
    if(setsockopt(_server->m_serverSocket.m_listeningSocket, SOL_SOCKET, SO_REUSEADDR, &optionValue, sizeof(optionValue)) < 0)
    {
        #ifdef USE_SERVER_LOGGER

        fprintf(_server->m_serverLogger, "%ld - [ERROR:] Failure while trying to configure the port to be reuseable...\n", time(NULL));

        #endif /* #ifdef USE_SERVER_LOGGER */

        return 0;
    }

    /* Bind the listening socket to the server's port */
    if(bind(_server->m_serverSocket.m_listeningSocket, (struct sockaddr*)&_server->m_serverSocket.m_serverSocketAddress, sizeof(_server->m_serverSocket.m_serverSocketAddress)) < 0)
    {
        #ifdef USE_SERVER_LOGGER

        fprintf(_server->m_serverLogger, "%ld - [ERROR:] Failure while trying to bind the server socket to the port...\n", time(NULL));

        #endif /* #ifdef USE_SERVER_LOGGER */

        return 0;
    }

    /* Listen to the server's port - make the socket passive */
    if(listen(_server->m_serverSocket.m_listeningSocket, _totalAvalableClientsSockets) < 0)
    {
        #ifdef USE_SERVER_LOGGER

        fprintf(_server->m_serverLogger, "%ld - [ERROR:] Failure while trying to listen the port...\n", time(NULL));

        #endif /* #ifdef USE_SERVER_LOGGER */

        return 0;
    }

    FD_ZERO(&_server->m_socketsSignalsIndicator);
    FD_SET(_server->m_serverSocket.m_listeningSocket, &_server->m_socketsSignalsIndicator);

    _server->m_isStopServerFromRunning = 0;
    _server->m_currentConnectedClientsCount = 0;

    #ifdef USE_SERVER_LOGGER

    fprintf(_server->m_serverLogger, "%ld - [INFO:] Successful server initialization\n", time(NULL));

    #endif /* #ifdef USE_SERVER_LOGGER */

    return 1;
}


static int InitializeServerSocket(ServerSocket* _serverSocket, FILE* _serverLogger, unsigned int _serverPort)
{
    _serverSocket->m_listeningSocket = socket(AF_INET, SOCK_STREAM, 0); /* TCP socket */
    if(_serverSocket->m_listeningSocket < 0)
    {
        #ifdef USE_SERVER_LOGGER

        fprintf(_serverLogger, "%ld - [ERROR:] Failure while trying to create a socket...\n", time(NULL));

        #endif /* #ifdef USE_SERVER_LOGGER */

        return 0;
    }

    memset(&_serverSocket->m_serverSocketAddress, 0, sizeof(_serverSocket->m_serverSocketAddress)); /* Clearing the socket address object first */
    _serverSocket->m_serverSocketAddress.sin_addr.s_addr = INADDR_ANY; /* Listening to every valid ip address */
    _serverSocket->m_serverSocketAddress.sin_port = htons(_serverPort);
    _serverSocket->m_serverSocketAddress.sin_family = AF_INET; /* IPv4 */

    return 1;
}


static ServerResult AcceptNewClients(Server* _server)
{
    HandlingClientResult result;
    Response response;
    Socket* socketPtr = NULL;
    struct sockaddr_in clientSocketAddress;
    ClientInfo clientInfo;
    Socket newClientSocket;
    unsigned int clientSocketAddressSize = sizeof(clientSocketAddress);

    if(_server->m_currentConnectedClientsCount == _server->m_maxAmountOfConnectedClientsAtSameTime)
    {
        return SERVER_SUCCESS; /* Cannot add more clients */
    }

    newClientSocket = accept(_server->m_serverSocket.m_listeningSocket, (struct sockaddr*)&clientSocketAddress, &clientSocketAddressSize);
    if(newClientSocket < 0)
    {
        #ifdef USE_SERVER_LOGGER

        fprintf(_server->m_serverLogger, "%ld - [ERROR:] Failure while trying to accept a new client...\n", time(NULL));

        #endif /* #ifdef USE_SERVER_LOGGER */

        return SERVER_ACCEPT_CLIENT_FAILED;
    }

    socketPtr = (Socket*)malloc(sizeof(Socket));
    if(!socketPtr)
    {
        close(newClientSocket);

        #ifdef USE_SERVER_LOGGER

        fprintf(_server->m_serverLogger, "%ld - [ERROR:] Failure while trying to allocate memory...\n", time(NULL));

        #endif /* #ifdef USE_SERVER_LOGGER */

        return SERVER_MEMORY_ALLOCATION_FAILED;
    }

    *socketPtr = newClientSocket;

    if(LinkedListInsertTail(_server->m_waitingConnectionSocketsList, (void*)socketPtr) != LINKEDLIST_SUCCESS)
    {
        close(newClientSocket);
        free(socketPtr);

        #ifdef USE_SERVER_LOGGER

        fprintf(_server->m_serverLogger, "%ld - [ERROR:] Failure while trying to allocate memory...\n", time(NULL));

        #endif /* #ifdef USE_SERVER_LOGGER */

        return SERVER_MEMORY_ALLOCATION_FAILED; /* Can ONLY be an allocation failure error */
    }

    /* Set it to the sockets signals indicator */
    FD_SET(newClientSocket, &_server->m_socketsSignalsIndicator);

    ++(_server->m_currentConnectedClientsCount);

    #ifdef USE_SERVER_LOGGER

    fprintf(_server->m_serverLogger, "%ld - [INFO:] Client %d has connected to the server\n", time(NULL), newClientSocket);

    #endif /* #ifdef USE_SERVER_LOGGER */

    if(_server->m_onNewClientConnection) /* This function can be NULL (if not needed) */
    {
        /* Initialize the client info object */
        clientInfo.m_clientID = newClientSocket; /* Using the client's socket number (file descriptor) as an ID of the client */
        strcpy(clientInfo.m_clientIPAddress, inet_ntoa(clientSocketAddress.sin_addr));
        clientInfo.m_clientPort = ntohs(clientSocketAddress.sin_port);

        response.m_clientID = newClientSocket;
        response.m_isMessageDeallocationRequired = 0;

        _server->m_onNewClientConnection(&clientInfo, &response, _server->m_applicationInfo);

        result = HandleResponse(&response);
        if(result == CLIENT_FINISH || result == CLIENT_ERROR)
        {
            /* LinkedListIteratorPrev(LinkedListIteratorEnd(_server->m_waitingConnectionSocketsList)) - will give the last inserted client */
            DisconnectAndRemoveClientFromServer(_server, LinkedListIteratorPrev(LinkedListIteratorEnd(_server->m_waitingConnectionSocketsList)), newClientSocket);
        }

        if(response.m_isMessageDeallocationRequired)
        {
            free(response.m_responseMessageContent);
        }

        /* Finished to handle the response from the application */
    }

    return SERVER_SUCCESS;
}


static void HandleExistingClientsRequests(Server* _server, fd_set _socketsSignalsIndicator, int _clientsSocketSignalsCount)
{
    int isServerShouldStopAfterHandlingAllClients = 0;
    Response response;
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
            result = HandleSingleClientRequest(*currentClientSocket, (void*)_server->m_receivedMessagesBuffer, _server->m_receivedMessagesBufferSize);
            if(result == CLIENT_FINISH || result == CLIENT_ERROR)
            {
                DisconnectAndRemoveClientFromServer(_server, iterator, *currentClientSocket);
            }
            else /* CLIENT_KEEP - message has successfully received for the client */
            {
                response.m_clientID = *currentClientSocket; /* Current socket number is the default value of the response */
                response.m_isMessageDeallocationRequired = 0;

                isServerShouldStopAfterHandlingAllClients = _server->m_onClientMessage((void*)_server->m_receivedMessagesBuffer, *currentClientSocket, &response, _server->m_applicationInfo);
                if(isServerShouldStopAfterHandlingAllClients)
                {
                    _server->m_isStopServerFromRunning = 1;
                }

                result = HandleResponse(&response); /* Send message /  */
                if(result == CLIENT_FINISH || result == CLIENT_ERROR)
                {
                    DisconnectAndRemoveClientFromServer(_server, iterator, *currentClientSocket);
                }

                if(response.m_isMessageDeallocationRequired)
                {
                    free(response.m_responseMessageContent);
                }

                /* Finished to handle the response from the application */
            }

            --_clientsSocketSignalsCount;

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


static void DisconnectAndRemoveClientFromServer(Server* _server, void* _elementToRemove, Socket _clientSocket)
{
    if(_server->m_onCloseClientConnection) /* This function can be NULL (if not needed) */
    {
        _server->m_onCloseClientConnection(_clientSocket, _server->m_applicationInfo);
    }

    LinkedListIteratorRemove(_elementToRemove);
    FD_CLR(_clientSocket, &_server->m_socketsSignalsIndicator);
    close(_clientSocket);

    --(_server->m_currentConnectedClientsCount);

    #ifdef USE_SERVER_LOGGER

    fprintf(_server->m_serverLogger, "%ld - [INFO:] Client %d has disconnected from the server\n", time(NULL), _clientSocket);

    #endif /* #ifdef USE_SERVER_LOGGER */
}


static HandlingClientResult HandleResponse(Response* _response)
{
    ssize_t bytes;
    size_t i;

    if(_response->m_responseStatus == RESPONSE_SEND_MESSAGE)
    {
        bytes = send(_response->m_clientID, _response->m_responseMessageContent, _response->m_responseMessageContentSize, 0);
        if(bytes < 0)
        {
            return CLIENT_ERROR;
        }
        else if(bytes != _response->m_responseMessageContentSize)
        {
            for(i = 0; i < TIMEOUT_VALUE; ++i)
            {
                bytes = send(_response->m_clientID, _response->m_responseMessageContent, _response->m_responseMessageContentSize, 0);
                if(bytes == _response->m_responseMessageContentSize)
                {
                    break; /* Succeed to send */
                }
            }
        }

        return CLIENT_KEEP;
    }
    else if(_response->m_responseStatus == RESPONSE_DISCONNECT_CLIENT)
    {
        return CLIENT_FINISH; /* Disconnect the client */
    }
    else /* RESPONSE_DO_NOTHING */
    {
        return CLIENT_KEEP;
    }
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


static const char* MapServerErrorsToMessages(ServerResult _statusCode)
{
    if(_statusCode == SERVER_MEMORY_ALLOCATION_FAILED)
    {
        return "Failed while tying to allocate memory...\n";
    }
    else if(_statusCode == SERVER_ACCEPT_CLIENT_FAILED)
    {
        return "Failed while trying to accept a new client...\n";
    }
    else if(_statusCode == SERVER_INTERNAL_ERROR)
    {
        return "Server internal error has occurred...\n";
    }
    else
    {
        return "Unknown error has occurred...\n";
    }
}