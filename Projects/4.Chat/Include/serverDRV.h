#ifndef __SERVERDRV_H__
#define __SERVERDRV_H__

#include <stddef.h>
#define CONTINU 1
#define STOP 0

typedef struct Server Server;

typedef struct ClientDetails
{
	size_t m_clientId; /*clientSocket*/
	size_t m_clientPort;
	char* m_clientIpAddress;
}ClientDetails;


typedef enum ServerResult
{
	SERVER_SUCCESS = 0,	
	SERVER_UNINITIALIZED,	/** Uninitialized Server*/
	SERVER_SOCKET_INITIALIZATION_FAILED,
	SERVER_SELECT_FAILED,
	SERVER_ACCEPT_FAILED, 
	SERVER_ALLOCATION_FAILED, 
	SERVER_RECEIVED_FROM_CLIENT_FAILED, 
	SERVER_SEND_MESSAGE_FAILED
}ServerResult;


typedef void (*NewClient)(Server* _server, int _clientSocket, void* _appContext);
/* you need to copy the buffer*/
typedef void (*GotMessage)(Server* _server, void* _message, int _SizeMessage, int _clientSock, void* _appContext);
typedef void (*CloseClient)(Server* _server, int _clientSocket, void* _appContext); 
/*if return value 0 server stop
	if is 1 continu*/
typedef int (*Fail)(ServerResult _error, void* _appContext);


typedef struct AppFunction
{
    NewClient m_newClientFunction;
    GotMessage m_gotMessageFunction;
    CloseClient m_closeClientFunction;
    Fail m_failFunction;
}AppFunction;







/**
 * @brief 
 * @param 
 * @return Server* or NULL
 */
Server* ServerCreate(AppFunction* _appFunction, size_t _queueSize, size_t _serverPort, size_t _maxNumFD, void *_appContext);


void ServerRun(Server* _server);





ServerResult ServerSendMessage(Server* _server, size_t _clientSocket, void* _message, size_t _messageSize);




void ServerDestroy(Server** _server);

#endif /*#ifndef __SERVERDRV_H__*/