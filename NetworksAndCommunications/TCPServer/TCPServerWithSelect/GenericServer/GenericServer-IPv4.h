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

#ifndef __GENERICSERVER_IPV4_H__
#define __GENERICSERVER_IPV4_H__

/* Defines: */

typedef struct Server Server;

typedef struct ClientInfo
{
    int m_clientID;
    int m_clientPort;
    char* m_clientIPAddress;
} ClientInfo;

typedef enum ServerResult
{
    SERVER_SUCCESS = 0,
    SERVER_NOT_INITIALIZED,
    SERVER_MEMORY_ALLOCATION_FAILED,
    SERVER_ACCEPT_CLIENT_FAILED,
    SERVER_INTERNAL_ERROR,
    SERVER_SEND_MESSAGE_FAILED,
} ServerResult;

/**
 * @brief A pointer to a function to be triggered as an handler when the server has received a message from a client
 * @param[in] _server: The Generic TCP Server that triggered the handler function
 * @param[in] _message: The received message from the client
 * @param[in] _client: The ID of the client that sent the message to the server
 * @param[in] _applicationInfo: The application context to be used in the handler function (that application context was given in the server's creation part)
 * @return None
 */
typedef void (*ClientMessageHandler)(Server* _server, void* _message, int _clientID, void* _applicationInfo);

/**
 * @brief A pointer to a function to be triggered as an handler when an error has occurred in the server
 * @param[in] _server: The Generic TCP Server that triggered the handler function
 * @param[in] _errorCode: The server's error status code
 * @param[in] _errorMessage: The error message of the server's error in string representation
 * @param[in] _applicationInfo: The application context to be used in the handler function (that application context was given in the server's creation part)
 * @return None
 */
typedef void (*ErrorHandler)(Server* _server, ServerResult _errorCode, const char* _errorMessage, void* _applicationInfo);

/**
 * @brief A pointer to a function to be triggered as an handler when a new client has connected to the server
 * @param[in] _server: The Generic TCP Server that triggered the handler function
 * @param[in] _clientInfo: The information of the new connected client
 * @param[in] _applicationInfo: The application context to be used in the handler function (that application context was given in the server's creation part)
 * @return None
 *
 * @warning: The _clientInfo object is a stack allocated variable, make sure to COPY its content, and DO NOT save it 'as is',
 *           Make sure as well to use some string copying function to copy the content of the m_clientIPAddress member into a string buffer
 */
typedef void (*NewClientConnectionHandler)(Server* _server, ClientInfo* _clientInfo/*tell the user to copy the ip*/, void* _applicationInfo);

/**
 * @brief A pointer to a function to be triggered as an handler when an existing client has close its connection to the server, or if the server had to close the connection with the client, can be NULL if such handling is not required
 * @param[in] _server: The Generic TCP Server that triggered the handler function
 * @param[in] _client: The ID of the client that its connection with the server had closed
 * @param[in] _applicationInfo: The application context to be used in the handler function (that application context was given in the server's creation part)
 * @return None
 */
typedef void (*CloseClientConnectionHandler)(Server* _server, int _clientID, void* _applicationInfo);


/* ------------------------------------ Main API Functions -------------------------------------- */

/**
 * @brief Creates a new Generic TCP Server (supports: IPv4)
 * @details The Server can receive four pointers to a handling functions,
 *          At least two of them must be initialized for the server's creation: OnClientMessage handler, and OnError handler
 * @param[in] _applicationInfo: An application info to be passed to every handler function, can be NULL if not required (ex. usage: a pointer to an object to be updated while using the handlers)
 * @param[in] _clientMessageHandler: A handler function to be triggered when the server had received a message from a client, cannot be NULL - the server must response to a client's received message
 * @param[in] _errorHandler: A handler function to be triggered when an error had occurred in the server, cannot be NULL - the server must response to an error
 * @param[in] _newClientConnectionHandler: A handler function to be triggered when a new client has connected to the server, can be NULL if such handling is not required
 * @param[in] _closeClientConnectionHandler: A handler function to be triggered when an existing client has close its connection to the server, or if the server had to close the connection with the client, can be NULL if such handling is not required
 * @param[in] _useServerLogger: A boolean flag to set if a server logger is required or not (the log file will be named: "GenericTCPServer.log")
 * @return Server* - on success / NULL - on failure
 *
 * @warning If _clientMessageHandler or _errorHandler are NULL: function will fail and return NULL
 * @warning The server initialization can fail because of various internal errors, if at least one of them occurs: function will fail and return NULL (and will NOT trigger the OnError handle function)
 */
Server* ServerCreate(void* _applicationInfo, ClientMessageHandler _onClientMessage, ErrorHandler _onError, NewClientConnectionHandler _onNewClientConnection, CloseClientConnectionHandler _onCloseClientConnection, int _useServerLogger);


/**
 * @brief Destroys a given Generic TCP Server, NULLs the Server's pointer to prevent double destroy attempts
 * @param[in] _server: A given Generic TCP Server to destroy
 * @return None
 */
void ServerDestroy(Server** _server);


/**
 * @brief Runs a given Generic TCP Server
 * @param[in] _server: A Generic TCP Server to run
 * @return None
 */
void ServerRun(Server* _server);


/**
 * @brief Forces a given Generic TCP Server to stop its running
 * @details The ServerForceStop function should be executed when the OnError handler is triggered
 * @param[in] _server: A Generic TCP Server to stop its running
 * @return None
 */
void ServerForceStop(Server* _server);


/**
 * @brief Sends a message from the Server to a specific client (identified by client ID)
 * @param[in] _server: A Generic TCP Server to send a message from
 * @param[in] _clientID: A client ID to identify the client to send the message to
 * @param[in] _message: A pointer to a buffer message to send to the client
 * @return ServerResult - on success or on failure
 * @retval SERVER_SUCCESS on success
 * @retval SERVER_NOT_INITIALIZED on error - a given pointer is not initialized
 * @retval SERVER_SEND_MESSAGE_FAILED on error - failed to send a given messages
 * @retval SERVER_WRONG_CLIENT_ID on error - a given client id is wrong
 *
 *@warning The _message is a stack allocated variable, make sure to COPY its content, and DO NOT save it 'as is',
 *         Make sure as well to use some string copying function (if _message is a string), to copy its content
 */
ServerResult ServerSendMessage(Server* _server, int _clientID, void* _message);

/* --------------------------------- End of Main API Functions ----------------------------------- */

#endif /* #ifndef __GENERICSERVER_IPV4_H__ */