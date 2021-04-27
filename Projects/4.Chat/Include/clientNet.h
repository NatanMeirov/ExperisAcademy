#ifndef __CLIENTNET_H__
#define __CLIENTNET_H__

#include <stddef.h> /*size_t*/

typedef struct Client Client;

typedef enum ClientResult
{
    CLIENT_SUCCESS,
    CLIENT_CONNECTION_FAILED,
    CLIENT_SEND_FAILED,
    CLIENT_SEND_NOT_ALL_MESSAGE
}ClientResult;


/**
 * @brief The function creates a client
 * @param _serverPort- Port of the server
 * @param _serverIp- IP address of the server
 * @return Client* or NULL on failed
 */
Client* ClientCreate(int _serverPort, char* _serverIp);



/**
 * @brief The function receives a message from the server into the sent buffer and returns the amount of bytes received     
 * @param _client- poinet to client
 * @param _buffer- A buffer within which the message will be saved
 * @param _bufferLen- size of the buffer
 * @return The amount of bytes received
 * @warning  If the amount of bytes received is 0 the server close the communication
 *           If it's negative the received failed
 */
int ClientReceiveMessage(Client* _client, void* _buffer, size_t _bufferLen);


/**
 * @brief The function sends a message to the server connected to the client
 * @param _client- poinet to client 
 * @param _message-  
 * @param _bufferLen
 * @return ClientResult: CLIENT_SUCCESS on success, CLIENT_SEND_NOT_ALL_MESSAGE on partial send of the message, CLIENT_SEND_FAILED on failed send
 */
ClientResult ClientSendMessage(Client* _client, void* _message, size_t _messageSize);

/**
 * @brief The function destroys the Client struct, provide double free protection
 * @param _client - pointer Client to destroy
 * @return NONE
 * @warning after destroy _client == NULL
 */
void ClientDestroy(Client** _client);

#endif /*#ifndef __CLIENTNET_H__*/