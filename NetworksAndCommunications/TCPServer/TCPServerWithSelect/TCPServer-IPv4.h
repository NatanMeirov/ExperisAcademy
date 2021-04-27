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

#ifndef __TCPSERVER_IPV4_H__
#define __TCPSERVER_IPV4_H__

/* Defines: */

typedef struct TCPServer TCPServer;

typedef enum TCPServerResult
{
    TCPSERVER_SUCCESS = 0,
    TCPSERVER_NOT_INITIALIZED,
    TCPSERVER_MEMORY_ALLOCATION_FAILED,
    TCPSERVER_SOCKET_CREATION_FAILED,
    TCPSERVER_INTERNAL_ERROR
} TCPServerResult;


/* -------------------------------- Main API Functions -------------------------------------- */

/**
 * @brief Creates a new TCP Server (supports: IPv4)
 * @return TCPServer* - on success / NULL - on failure
 */
TCPServer* TCPServerCreate();


/**
 * @brief Runs a given TCP Server
 * @param[in] _server: A TCP Server to run
 * @return TCPServerResult - on success or on failure
 * @retval TCPSERVER_SUCCESS on success
 * @retval TCPSERVER_NOT_INITIALIZED on error - TCPServer pointer is not initialized
 * @retval TCPSERVER_MEMORY_ALLOCATION_FAILED on error - failed to allocate more memory
 */
TCPServerResult TCPServerRun(TCPServer* _server);


/**
 * @brief Destroys a given TCP Server, NULLs the TCPServer's pointer to prevent double destroy attempts
 * @param[in] _server: A given TCP Server to destroy
 * @return None
 */
void TCPServerDestroy(TCPServer** _server);

/* ----------------------------- End of Main API Functions ----------------------------------- */

#endif /* #ifndef __TCPSERVER_IPV4_H__ */