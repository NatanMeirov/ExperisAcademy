#include "../Include/clientNet.h"
#include <sys/socket.h> /*socket*/
#include <netinet/in.h> /*sockaddr_in*/
#include <string.h> /*memset*/
#include <stdlib.h> /*malloc, free*/
#include <arpa/inet.h> /* inet_addr */
#include <unistd.h> /* close */

#define TIME_OUT 10

struct  Client
{
    int m_clientSock;
};



Client* ClientCreate(int _serverPort, char* _serverIp)
{
    Client* client;
    struct sockaddr_in sin;
    int sock;
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock < 0)
    {
        return NULL;
    }

    memset(&sin, 0, sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = inet_addr(_serverIp); /*IP SERVER*/
    sin.sin_port = htons(_serverPort);
    if(connect(sock, (struct sockaddr*) &sin, sizeof(sin)) < 0)
    {
        close(sock);
        return NULL;
    }

    client = (Client*)malloc(sizeof(Client));
    if(client == NULL)
    {
        close(sock);
        return NULL;
    }
    client->m_clientSock = sock;
    return client;
}




void ClientDestroy(Client** _client)
{
    if(_client != NULL && *_client != NULL)
    {
        close((*_client)->m_clientSock);
        free(*_client);
        *_client = NULL;
    }
}


int ClientReceiveMessage(Client* _client, void* _buffer, size_t _bufferLen)
{
    int readBytes = recv(_client->m_clientSock, _buffer, _bufferLen, 0);
    return readBytes;
}



ClientResult ClientSendMessage(Client* _client, void* _message, size_t _messageSize)
{
    int i=0;
    int sendBytes = send(_client->m_clientSock, _message, _messageSize, 0);
    if(sendBytes < 0)
    {
        return CLIENT_SEND_FAILED;
    }
    while(sendBytes != _messageSize)
    {
        sendBytes = send(_client->m_clientSock, _message, _messageSize, 0);
        if(sendBytes < 0)
        {
            return CLIENT_SEND_NOT_ALL_MESSAGE;
        }
        if(i == TIME_OUT && sendBytes != _messageSize)
        {
            return CLIENT_SEND_NOT_ALL_MESSAGE;
        }
        i++;
    }
    return CLIENT_SUCCESS;
}