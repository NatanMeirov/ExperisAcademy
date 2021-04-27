/**
 * @file GenericServer-IPv4.h
 * @author Natan Meirov (NatanMeirov@gmail.com)
 * @brief A Generic TCP Server implementation, supports IPv4 only
 * In Version 2: improved performance of the server driver
 * Note: To use the server's logger - make sure to define: USE_SERVER_LOGGER constant
 * Note: This server can handle maximum of 1020 clients only at same time (because of a limitation of the specific implementation, using Linux's select system call!)
 * Note: This server can receive a 4K bits message (for each single message)
 * @version 2.0
 * @date 2021-04-22
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#ifndef __GENERICSERVER_IPV4_H__
#define __GENERICSERVER_IPV4_H__


/* Includes: */

#include <stddef.h> /* size_t */


/* Defines: */

#define IPV4_LENGTH 16

typedef struct Server Server;

typedef struct ClientInfo
{
    int m_clientID;
    int m_clientPort;
    char m_clientIPAddress[IPV4_LENGTH];
} ClientInfo;

typedef enum ResponseStatus
{
    RESPONSE_DO_NOTHING = 0,
    RESPONSE_SEND_MESSAGE,
    RESPONSE_DISCONNECT_CLIENT
} ResponseStatus;

typedef struct Response
{
    ResponseStatus m_responseStatus; /* The operation that the server should do, if an operation not specified (or wrong value) - the server will use DO_NOTHING command */
    int m_clientID; /* The client ID of the client to send the respone to (default value: the client ID of the client that the server received a message from) */
    void* m_responseMessageContent; /* The message to send to the client */
    size_t m_responseMessageContentSize; /* The message's size */
    int m_isMessageDeallocationRequired; /* A bool flag to tell the server if deallocation (free) is required for the message (default value: false) */
} Response;

typedef enum ServerResult
{
    SERVER_SUCCESS = 0,
    SERVER_NOT_INITIALIZED,
    SERVER_MEMORY_ALLOCATION_FAILED,
    SERVER_ACCEPT_CLIENT_FAILED,
    SERVER_INTERNAL_ERROR
} ServerResult;

/**
 * @brief A pointer to a function to be triggered as an handler when the server has received a message from a client
 * @param[in] _message: The received message from the client
 * @param[in] _client: The ID of the client that sent the message to the server
 * @param[in] _response: The response (object) that the server should use to send a response to the client, it should include:
 *                       - The response status to tell the server which operation it should do: RESPONSE_DO_NOTHING, RESPONSE_SEND_MESSAGE, RESPONSE_DISCONNECT_CLIENT (with the specified ID of the response object) [default value or other input - the server will do nothing]
 *                       - The client ID to send the response message to (default value: the client ID that the server received the message from)
 *                       - The message to send to the client with that ID
 *                       - The size of the message to send to the client
 *                       - A boolean flag to tell the server if a deallocation (free) is required for the message after its sending (default value: false)
 * @param[in] _applicationInfo: The application context to be used in the handler function (that application context was given in the server's creation part)
 * @return None
 *
 * @warning The message will be a stream of received bytes (unsigned chars), that stored is a stack allocated buffer (buffer size: 4K bits [4096])
 */
typedef void (*ClientMessageHandler)(void* _message, int _clientID, Response* _response, void* _applicationInfo);

/**
 * @brief A pointer to a function to be triggered as an handler when an error has occurred in the server, the error handler should return a bool value to tell the server how it should operate (0, if the server should NOT stop, else 1 if it HAVE TO stop its running)
 * @param[in] _errorCode: The server's error status code
 * @param[in] _errorMessage: The error message of the server's error in string representation
 * @param[in] _applicationInfo: The application context to be used in the handler function (that application context was given in the server's creation part)
 * @return 1 - if the server should stop its running / 0 - if the server should continue its running
 */
typedef int (*ErrorHandler)(ServerResult _errorCode, const char* _errorMessage, void* _applicationInfo);

/**
 * @brief A pointer to a function to be triggered as an handler when a new client has connected to the server
 * @param[in] _clientInfo: The information of the new connected client
 * @param[in] _response: A response (object) that the server should use to send a response to the client, (if a response is needed, if not - just use RESPONSE_DO_NOTHING), it should include:
 *                       - The response status to tell the server which operation it should do: RESPONSE_DO_NOTHING, RESPONSE_SEND_MESSAGE, RESPONSE_DISCONNECT_CLIENT (with the specified ID of the response object) [default value or other input - the server will do nothing]
 *                       - The client ID to send the response message to (default value: the client ID that the server received the message from)
 *                       - The message to send to the client with that ID
 *                       - The size of the message to send to the client
 *                       - A boolean flag to tell the server if a deallocation (free) is required for the message after its sending (default value: false)
 * @param[in] _applicationInfo: The application context to be used in the handler function (that application context was given in the server's creation part)
 * @return None
 *
 * @warning The _clientInfo object is a stack allocated variable, make sure to COPY its content, and DO NOT save it 'as is',
 *          Make sure as well to use some string copying function to copy the content of the m_clientIPAddress member into a string buffer
 */
typedef void (*NewClientConnectionHandler)(ClientInfo* _clientInfo, Response* _response, void* _applicationInfo);

/**
 * @brief A pointer to a function to be triggered as an handler when an existing client has close its connection to the server, or if the server had to close the connection with the client, can be NULL if such handling is not required
 * @param[in] _client: The ID of the client that its connection with the server had closed
 * @param[in] _applicationInfo: The application context to be used in the handler function (that application context was given in the server's creation part)
 * @return None
 */
typedef void (*CloseClientConnectionHandler)(int _clientID, void* _applicationInfo);


/* ------------------------------------ Main API Functions -------------------------------------- */

/* *** Note: To use the server's logger - make sure to define: USE_SERVER_LOGGER constant [The log file will be called: "GenericTCPServer.log", and will be opened with append mode]*** */

/**
 * @brief Creates a new Generic TCP Server (supports: IPv4)
 * @details The Server can receive four pointers to a handling functions,
 *          At least two of them must be initialized for the server's creation: OnClientMessage handler, and OnError handler
 * @param[in] _applicationInfo: An application info to be passed to every handler function, can be NULL if not required (ex. usage: a pointer to an object to be updated while using the handlers)
 * @param[in] _clientMessageHandler: A handler function to be triggered when the server had received a message from a client, cannot be NULL - the server must response to a client's received message
 * @param[in] _errorHandler: A handler function to be triggered when an error had occurred in the server, cannot be NULL - the server must response to an error
 * @param[in] _newClientConnectionHandler: A handler function to be triggered when a new client has connected to the server, can be NULL if such handling is not required
 * @param[in] _closeClientConnectionHandler: A handler function to be triggered when an existing client has close its connection to the server, or if the server had to close the connection with the client, can be NULL if such handling is not required
 * @param[in] _serverPort: A port for the server, must be between 1025 to 64K
 * @param[in] _maxWaitingConnections: The maximum amount of waiting connections (sockets), used to initialize the OS waiting connections queue, must be more then 0
 * @return Server* - on success / NULL - on failure
 *
 * @warning If _clientMessageHandler or _errorHandler are NULL: function will fail and return NULL
 * @warning If _serverPort is not between 1025 to 64K: function will fail and return NULL
 * @warning If _maxWaitingConnections is <= 0: function will fail and return NULL
 * @warning The server initialization can fail because of various internal errors, if at least one of them occurs: function will fail and return NULL (and will NOT trigger the OnError handle function)
 */
Server* ServerCreate(void* _applicationInfo, ClientMessageHandler _onClientMessage, ErrorHandler _onError, NewClientConnectionHandler _onNewClientConnection, CloseClientConnectionHandler _onCloseClientConnection, unsigned int _serverPort, int _maxWaitingConnections);


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

/* --------------------------------- End of Main API Functions ----------------------------------- */

#endif /* #ifndef __GENERICSERVER_IPV4_H__ */