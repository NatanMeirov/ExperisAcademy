/**
 * @file NMSocket.h
 * @author Natan Meirov (NatanMeirov@gmail.com)
 * @brief An advanced socket API module that wraps the C standard socket library
 * Handles socket operations for both - server side and client side
 * @version 1.0
 * @date 2021-04-19
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#ifndef __NMSOCKET_H__
#define __NMSOCKET_H__

/* Includes: */

#include <sys/types.h> /* ssize_t, size_t */


/* Defines: */

typedef struct NMSocket NMSocket;

typedef enum IpVersion { IPV4, IPV6 } IpVersion; /* Supported: IP version 4 / IP version 6 Internet Protocols */

typedef enum TransportProtocol { TCP, UDP } TransportProtocol; /* Supported: TCP / UDP Transport Protocols */

typedef enum SocketType { SERVER, CLIENT } SocketType;

typedef struct SocketAddressData
{
    char* m_ipAddress; /* If NULL or "any"(lower case): All available IP addresses (usually - used by servers), If "localhost" or "loopback" (lower case): Loopback address (127.0.0.1) */
    unsigned int m_port; /* Send/Receive port number */
} SocketAddressData;

/* ------------------------------------- Main API Functions ----------------------------------- */

/**
 * @brief 
 * There are 4 available socket combinations: UDP-Client / UDP-Server / TCP-Client / TCP-Server
 * @param _ipVersion 
 * @param _protocol 
 * @param _socketType 
 * @param _socketAddressData 
 * @param _waitingConnectionsCap: Used ONLY for TCP Servers Socket, to initialize the waiting connection (back log) of OS queue, with a max cap length, cannot be 0 (if not creating a TCP Server socket - should be 0)
 * @return NMSocket* - on success / NULL - on failure
 * (optional errors: wrong ip version, wrong protocol, allocation failed, wrong port (blow 1024))
 */
NMSocket* NMSocketCreate(IpVersion _ipVersion, TransportProtocol _protocol, SocketType _socketType, SocketAddressData _socketAddressData, size_t _waitingConnectionsCap);


/**
 * @brief Destroys a given NMSocket and closes its communications, NULLs the Socket's pointer
 * @param _socket: A socket to destroy
 * @return None
 */
void NMSocketDestroy(NMSocket** _socket);


/**
 * @brief 
 * @details Usually used by Clients - to set a new socket address of a new server/client address to communicate with, based on the original socket's ip version
 * @param _newSocketAddressData 
 * @return int - 1, os success / 0, on failure
 */
int NMSocketUpdateSocketAddress(NMSocket* _socket, SocketAddressData _newSocketAddressData);


/**
 * @brief Sends a given data buffer of some given size (in bytes), to the NEXT waiting message (TCP Server) side, or LAST received message (UDP Server), or original socket address [from the initialization part] (Clients)
 * @details If sending a data buffer to the last received socket address, use NMSocketGetLastReceivedSocketAddress function and then update the socket address to send by using NMSocketUpdateSocketAddress function
 * @param _socket 
 * @param _dataBufferToSend 
 * @param _bufferLengthInBytes 
 * @return ssize_t - number of sent bytes, on success / -1, on failure
 */
ssize_t NMSocketSend(NMSocket* _socket, const void* _dataBufferToSend, size_t _bufferLengthInBytes);


/**
 * @brief 
 * @param _socket 
 * @param _bufferToReceiveData 
 * @param _bufferLengthInBytes 
 * @return ssize_t - number of received bytes, on success / -1, on failure
 */
ssize_t NMSocketReceive(NMSocket* _socket, void* _bufferToReceiveData, size_t _bufferLengthInBytes);

/* ---------------------------------- End of Main API Functions -------------------------------- */

#endif /* #ifndef __NMSOCKET_H__ */