#include <sys/select.h> /*fd_set*/
#include <stdlib.h> /*abort, malloc, free*/
#include <netinet/in.h> /*sockaddr_in*/
#include <string.h> /*strcpy, memset*/
#include <sys/socket.h> /* C std socket lib */
#include <unistd.h> /* close */

#include "../Include/serverDRV.h"
#include "../Include/doubleLinkedList.h"
#include "../Include/ListItr.h"

#define MAX_FD 1023

struct Server
{
    int m_flag;
    size_t m_queueSize;
    size_t m_serverPort;
    size_t m_listenSock;
    size_t m_maxNumFD; /*the highest-numbered file descriptor*/
    fd_set m_master;
    List* m_socketsList;
    NewClient m_newClientFunc;
    GotMessage m_gotMessageFunc;
    CloseClient m_closeClientFunc;
    Fail m_failFunc;
    void* m_appContext;
};


/*help function*/
static int ServerTCPInitialization(Fail _failFunc, size_t _serverPort, size_t _queueSize);
static void ListElementDestroy(void* _element);
static int ServerTCPConnect(Fail _failFunc, int _sock);
static ServerResult AddNewClient(Server* _server);
static void CheckClients(Server* _server, int _activity, fd_set _readfds);



Server* ServerCreate(AppFunction* _appFunction, size_t _queueSize, size_t _serverPort, size_t _maxNumFD, void *_appContext)
{
    Server* server;
    if(_queueSize == 0 || _serverPort < 1024 || _appFunction == NULL || _appFunction->m_gotMessageFunction == NULL  || _maxNumFD < 2 || _maxNumFD > MAX_FD)
    {
        return NULL;
    }
    server = (Server*)malloc(sizeof(Server));
    if(server == NULL)
    {
        return NULL;
    }
    server->m_socketsList = ListCreate();
    if(server->m_socketsList == NULL)
    {
        free(server);
        return NULL;
    }
    server->m_flag = 1;
    server->m_queueSize = _queueSize; /*size of the queue*/
    server->m_serverPort = _serverPort;
    server->m_appContext = _appContext;
    FD_ZERO(&server->m_master);
    server->m_maxNumFD = _maxNumFD;
    server->m_newClientFunc = _appFunction->m_newClientFunction;
    server->m_gotMessageFunc = _appFunction->m_gotMessageFunction;
    server->m_closeClientFunc = _appFunction->m_closeClientFunction;
    server->m_failFunc = _appFunction->m_failFunction;
    server->m_listenSock = ServerTCPInitialization(_appFunction->m_failFunction, _serverPort, _queueSize);
    if(server->m_listenSock == 0)
    {
        ListDestroy(&(server->m_socketsList), NULL);
        free(server);
        return NULL;
    }
    FD_SET(server->m_listenSock, &(server->m_master));
    return server;
}


void ServerDestroy(Server** _server)
{
    if(_server == NULL || *_server == NULL)
    {
        return;
    }
    ListDestroy(&((*_server)->m_socketsList), &ListElementDestroy);
    close((*_server)->m_listenSock);
    free(*_server);
    *_server = NULL;
}


/*run when flag == 1*/
void ServerRun(Server* _server)
{
    int numOfSockActivity;
    fd_set readfds;
    ServerResult result;
    if(_server == NULL)
    {
        return;
    }

    while(_server->m_flag == 1)
    {
        readfds = _server->m_master;
        numOfSockActivity = select(_server->m_maxNumFD+1, &readfds, NULL, NULL, NULL);
        if(numOfSockActivity < 0)
        {
            if(_server->m_failFunc != NULL)
            {
               _server->m_flag = _server->m_failFunc(SERVER_SELECT_FAILED, _server->m_appContext);
               if(_server->m_flag == 0)
               {
                   break;
               }
            }
        }

        if(FD_ISSET(_server->m_listenSock, &readfds) == 1)
        {
            if((result = AddNewClient(_server)) != SERVER_SUCCESS && _server->m_failFunc != NULL)
            {
                _server->m_failFunc(result, _server->m_appContext);
            }
            numOfSockActivity--;
        }
        if(numOfSockActivity > 0)
        {
            CheckClients(_server, numOfSockActivity, readfds);
        }
    }
}



ServerResult ServerSendMessage(Server* _server, size_t _clientSocket, void* _message, size_t _messageSize)
{
    int sendBytes;
    if(_server == NULL || _message == NULL)
    {
        return SERVER_UNINITIALIZED;
    }
    sendBytes = send(_clientSocket, _message, _messageSize, 0);
    if(sendBytes < 0)
    {
        return SERVER_SEND_MESSAGE_FAILED;
    }
    return SERVER_SUCCESS;
}




















/*************** static function ***************/

static int ServerTCPInitialization(Fail _failFunc, size_t _serverPort, size_t _queueSize)
{
    int sock, optval;
    struct sockaddr_in sin;
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock < 0)
    {
        return 0;
    }

    optval = 1;
    if(setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) < 0)
    {
        close(sock);
        return 0;
    }

    memset(&sin, 0, sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = INADDR_ANY;
    sin.sin_port = htons(_serverPort);
    if(bind(sock, (struct sockaddr*)&sin, sizeof(sin)) < 0)
    {
        close(sock);
        return 0;
    }

    if(listen(sock, _queueSize) < 0)
    {
        close(sock);
        return 0;
    }
    return sock;
}

static void ListElementDestroy(void* _element)
{
    close(*((int*)_element));
    free(_element);
}


static ServerResult AddNewClient(Server* _server)
{
    int * temp;
    int clientSock;
    clientSock = ServerTCPConnect(_server->m_failFunc, _server->m_listenSock); 
    FD_SET(clientSock, &(_server->m_master));

    if(clientSock < 0)
    {
        return SERVER_ACCEPT_FAILED;
    }
    else
    {
        temp = (int*)malloc(sizeof(int));
        if(temp == NULL)
        {
            close(clientSock);
            return SERVER_ALLOCATION_FAILED;
        }
        *temp = clientSock;
        if(ListPushHead(_server->m_socketsList, temp) != LIST_SUCCESS)
        {
            free(temp);
            close(clientSock);
            return SERVER_ALLOCATION_FAILED;
        }
        if(_server->m_newClientFunc != NULL)
        {
            _server->m_newClientFunc(_server, clientSock, _server->m_appContext);
        }
    }
    return SERVER_SUCCESS;
}

static int ServerTCPConnect(Fail _failFunc, int _sock)
{
    int clientSock;
    struct sockaddr_in clientSin;
    socklen_t addrLen = sizeof(struct sockaddr_in);
    clientSock = accept(_sock, (struct sockaddr*) &clientSin, &addrLen);
    return clientSock;
}


static void CheckClients(Server* _server, int _activity, fd_set _readfds)
{
    ListItr begin, end, temp;
    int* clientSock;
    int * removeSock;
    int readBytes;

    
    char buffer[4096];
    begin = ListItrBegin(_server->m_socketsList);
    end = ListItrEnd(_server->m_socketsList);
    while(begin != end && _activity > 0)
    {
        temp = ListItrNext(begin);
        clientSock = ListItrGet(begin);
        if(FD_ISSET(*clientSock, &_readfds) == 0)
        {
            begin = ListItrNext(begin);
            continue;
        }

        readBytes = recv(*clientSock, buffer, 4096, 0);
        if(readBytes < 1)
        {
            removeSock = (int*)ListItrRemove(begin);
            close(*removeSock);
            FD_CLR(*removeSock, &(_server->m_master));
            if(_server->m_closeClientFunc != NULL)
            {
                _server->m_closeClientFunc(_server, *removeSock, _server->m_appContext);
            }
            free(removeSock);
            _activity--;
        }
        else
        {
            _server->m_gotMessageFunc(_server, buffer, readBytes, *clientSock, _server->m_appContext);
            _activity--;
        }
        
        begin = temp;
    }
}