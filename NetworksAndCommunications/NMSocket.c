/**
 * @file NMSocket.c
 * @author Natan Meirov (NatanMeirov@gmail.com)
 * @brief An advanced socket API module that wraps the C standard socket library
 * Handles socket operations for both - server side and client side
 * @version 1.0
 * @date 2021-04-19
 * 
 * @copyright Copyright (c) 2021
 * 
 */


/* Includes: */

#include <stddef.h> /* size_t, NULL */
#include <stdlib.h> /* malloc, free */
#include <string.h> /* memset, strlen, strcmp */
#include <unistd.h> /* close */
#include <sys/types.h> /* ssize_t */
#include <sys/socket.h> /* standard C socket lib to wrap */
#include <netinet/in.h> /* struct sockaddr_in, struct sockaddr_in6 */
#include <arpa/inet.h> /* inet_addr, inet_pton */
#include <asm-generic/socket.h> /* SOL_SOCKET, SO_REUSEADDR */
/*#include "../../Libraries/DynamicLibraries/GenericDataStructures/Include/GenericVector/GenericVector.h"*/ /* Generic DS */
#include "NMSocket.h"


/* Defines: */

#define MIN_VALID_PORT_NUM 1024
#define BUFFER_SIZE INET6_ADDRSTRLEN
#define MAX_OS_WAITING_SOCKETS_LIMIT 1020 /* For linux - can be changed */

typedef struct SocketAddress
{
    struct sockaddr_in* m_socketAddressIPV4;
    struct sockaddr_in6* m_socketAddressIPV6;
} SocketAddress;

typedef struct ServerSocketData /* Used for Servers only */
{
    /*Vector* m_waitingReceivedSockets;*/ /* Some data structure (maybe LList) to save the SOCKETS for TCP Server (dont need the sockaddr_in - OS is handling with the received socket address) */
    SocketAddress m_lastReceivedSocketAddress; /* Used for UDP server only */
} ServerSocketData;

struct NMSocket
{
    int m_socketID;
    int m_ipVersion; /* IPV4 (AF_INET) or IPV6 (AF_INET6) */
    int m_transportProtocol; /* UDP (SOCK_DGRAM) or TCP (SOCK_STREAM) */
    int m_isServer; /* 1 (true) if it is a server, else 0 (false) */
    ServerSocketData m_serverData; /* Used for Servers only */
    SocketAddress m_socketAddress;
};


/* Static Functions Declarations: */

static int ValidateNMSocketCreationParams(IpVersion _ipVersion, TransportProtocol _protocol, SocketType _socketType, SocketAddressData _socketData, size_t _waitingConnectionsCap);
static int InitializeSocketAddress(SocketAddress* _socketAddress, IpVersion _ipVersion, const char* _ipAddress, unsigned int _port);
static int SetServerProperties(NMSocket* _socket, size_t _waitingConnectionsCap);
static int ResetLastReceivedSocketAddress(SocketAddress* _socketAddress, IpVersion _ipVersion);


/* ------------------------------------- Main API Functions ----------------------------------- */


NMSocket* NMSocketCreate(IpVersion _ipVersion, TransportProtocol _protocol, SocketType _socketType, SocketAddressData _socketAddressData, size_t _waitingConnectionsCap)
{
    int ipVersionConst, protocolConst;
    NMSocket* nmSocket = NULL;

    if(!ValidateNMSocketCreationParams(_ipVersion, _protocol, _socketType, _socketAddressData, _waitingConnectionsCap))
    {
        return NULL;
    }

    nmSocket = malloc(sizeof(NMSocket));
    if(!nmSocket)
    {
        return NULL;
    }

    nmSocket->m_ipVersion = _ipVersion;
    nmSocket->m_transportProtocol = _protocol;

    if(_ipVersion == IPV4)
    {
        ipVersionConst = AF_INET;
    }
    else /* IPV6 */
    {
        ipVersionConst = AF_INET6;
    }

    if(_protocol == TCP)
    {
        protocolConst = SOCK_STREAM;
    }
    else /* UDP */
    {
        protocolConst = SOCK_DGRAM;
    }

    nmSocket->m_socketID = socket(ipVersionConst, protocolConst, 0);
    if(nmSocket->m_socketID < 0)
    {
        return NULL;
    }

    if(!InitializeSocketAddress(&nmSocket->m_socketAddress, _ipVersion, _socketAddressData.m_ipAddress, _socketAddressData.m_port))
    {
        return NULL;
    }

    nmSocket->m_serverData.m_lastReceivedSocketAddress.m_socketAddressIPV4 = NULL;
    nmSocket->m_serverData.m_lastReceivedSocketAddress.m_socketAddressIPV6 = NULL;

    if(_socketType == SERVER) /* Binding is needed */
    {
        nmSocket->m_isServer = 1;

        if(!SetServerProperties(nmSocket, _waitingConnectionsCap))
        {
            return NULL;
        }
    }
    else /* CLIENT */
    {
        nmSocket->m_isServer = 0;
    }

    return nmSocket;
}


void NMSocketDestroy(NMSocket** _socket)
{
    if(_socket && *_socket)
    {
        if((*_socket)->m_ipVersion == IPV4)
        {
            free((*_socket)->m_socketAddress.m_socketAddressIPV4);
        }
        else /* IPV6 */
        {
            free((*_socket)->m_socketAddress.m_socketAddressIPV6);
        }

        if((*_socket)->m_isServer)
        {
            if((*_socket)->m_transportProtocol == TCP)
            {
                /* TODO - FREE DATA STRUCTURE WHEN IMPLEMENTING IT! */
                /* FIXME - AND CLOSE EVERY WAITING SOCKET IN THE DS!!!! */
            }
            else /* UDP */
            {
                if((*_socket)->m_ipVersion == IPV4)
                {
                    free((*_socket)->m_serverData.m_lastReceivedSocketAddress.m_socketAddressIPV4);
                }
                else /* IPV6 */
                {
                    free((*_socket)->m_serverData.m_lastReceivedSocketAddress.m_socketAddressIPV6);
                }
            }
        }

        close((*_socket)->m_socketID); /* Could also use shutdown() */
        free((*_socket));
        *_socket = NULL;
    }
}


int NMSocketUpdateSocketAddress(NMSocket* _socket, SocketAddressData _newSocketAddressData)
{
    if(!_socket)
    {
        return 0;
    }

    if(_socket->m_socketAddress.m_socketAddressIPV4) /* IPV4 */
    {
        free(_socket->m_socketAddress.m_socketAddressIPV4);
    }
    else /* IPV6 */
    {
        free(_socket->m_socketAddress.m_socketAddressIPV6);
    }

    return InitializeSocketAddress(&_socket->m_socketAddress, _socket->m_ipVersion, _newSocketAddressData.m_ipAddress, _newSocketAddressData.m_port);
}


ssize_t NMSocketSend(NMSocket* _socket, const void* _dataBufferToSend, size_t _bufferLengthInBytes)
{
    if(!_socket || !_dataBufferToSend)
    {
        return -1;
    }

    if(_socket->m_isServer)
    {
        if(_socket->m_transportProtocol == TCP) /* TCP Server handling */
        {
            /* TODO - handle TCP send methods  (get one of the saved socketID from DS, and send a data to) - handle TCP server */
            return -1;
        }
        else /* UDP Server handling - sending to last receiver */
        {
            /* Check if Server did not receive any message till now */
            if(!_socket->m_serverData.m_lastReceivedSocketAddress.m_socketAddressIPV4 && !_socket->m_serverData.m_lastReceivedSocketAddress.m_socketAddressIPV6)
            {
                return -1;
            }

            if(_socket->m_ipVersion == IPV4)
            {
                return sendto(_socket->m_socketID, _dataBufferToSend, _bufferLengthInBytes, 0, (struct sockaddr*)(_socket->m_serverData.m_lastReceivedSocketAddress.m_socketAddressIPV4), sizeof(*(_socket->m_serverData.m_lastReceivedSocketAddress.m_socketAddressIPV4)));
            }
            else /* IPV6 */
            {
                return sendto(_socket->m_socketID, _dataBufferToSend, _bufferLengthInBytes, 0, (struct sockaddr*)(_socket->m_serverData.m_lastReceivedSocketAddress.m_socketAddressIPV6), sizeof(*(_socket->m_serverData.m_lastReceivedSocketAddress.m_socketAddressIPV6)));
            }
        }
    }
    else /* Client */
    {
        if(_socket->m_transportProtocol == TCP) /* TCP Client handling - sending to original initialized socket address */
        {
            if(_socket->m_ipVersion == IPV4)
            {
                /* First - establish a connection */
                if (connect(_socket->m_socketID, (struct sockaddr *) _socket->m_socketAddress.m_socketAddressIPV4, sizeof(*(_socket->m_socketAddress.m_socketAddressIPV4))) < 0)
                {
                    return -1;
                }
            }
            else /* IPV6 */
            {
                /* First - establish a connection */
                if (connect(_socket->m_socketID, (struct sockaddr *) _socket->m_socketAddress.m_socketAddressIPV6, sizeof(*(_socket->m_socketAddress.m_socketAddressIPV6))) < 0)
                {
                    return -1;
                }
            }

            return send(_socket->m_socketID, _dataBufferToSend, _bufferLengthInBytes, 0);
        }
        else /* UDP Client handling - sending to original initialized socket address */
        {
            if(_socket->m_ipVersion == IPV4)
            {
                return sendto(_socket->m_socketID, _dataBufferToSend, _bufferLengthInBytes, 0, (struct sockaddr*)(_socket->m_socketAddress.m_socketAddressIPV4), sizeof(*(_socket->m_socketAddress.m_socketAddressIPV4)));
            }
            else /* IPV6 */
            {
                return sendto(_socket->m_socketID, _dataBufferToSend, _bufferLengthInBytes, 0, (struct sockaddr*)(_socket->m_socketAddress.m_socketAddressIPV6), sizeof(*(_socket->m_socketAddress.m_socketAddressIPV6)));
            }
        }
    }
}


ssize_t NMSocketReceive(NMSocket* _socket, void* _bufferToReceiveData, size_t _bufferLengthInBytes)
{
    SocketAddress tempSocketAddress;
    unsigned int socketAddressSize;
    ssize_t returnedVal = -1;

    if(!_socket || !_bufferToReceiveData)
    {
        return -1;
    }

    if(_socket->m_isServer)
    {
        if(_socket->m_transportProtocol == TCP)
        {
            /* TODO - handle TCP receive methods (accept(), save socketID to DS) - handle server */
        }
        else /* UDP Server */
        {
            if(_socket->m_ipVersion == IPV4)
            {
                socketAddressSize = sizeof(*(_socket->m_serverData.m_lastReceivedSocketAddress.m_socketAddressIPV4));
            }
            else /* IPV6 */
            {
                socketAddressSize = sizeof(*(_socket->m_serverData.m_lastReceivedSocketAddress.m_socketAddressIPV4));
            }

            /* Reseting socket address - using the last received socket address space to read the received message () */
            if(!ResetLastReceivedSocketAddress(&_socket->m_serverData.m_lastReceivedSocketAddress, _socket->m_ipVersion))
            {
                return -1;
            }

            if(_socket->m_ipVersion == IPV4)
            {
                returnedVal = recvfrom(_socket->m_socketID, _bufferToReceiveData, _bufferLengthInBytes, 0, (struct sockaddr*)(tempSocketAddress.m_socketAddressIPV4), &socketAddressSize);
                free(tempSocketAddress.m_socketAddressIPV4);
            }
            else /* IPV6 */
            {
                returnedVal = recvfrom(_socket->m_socketID, _bufferToReceiveData, _bufferLengthInBytes, 0, (struct sockaddr*)(tempSocketAddress.m_socketAddressIPV6), &socketAddressSize);
                free(tempSocketAddress.m_socketAddressIPV6);
            }
        }
    }
    else /* Client */
    {
        if(_socket->m_transportProtocol == TCP)
        {
            /* TODO - handle TCP receive methods - handle client */
        }
        else /* UDP */
        {
            if(_socket->m_ipVersion == IPV4)
            {
                socketAddressSize = sizeof(*(_socket->m_socketAddress.m_socketAddressIPV4));
            }
            else /* IPV6 */
            {
                socketAddressSize = sizeof(*(_socket->m_socketAddress.m_socketAddressIPV6));
            }

            /* Reseting socket address - to use the temp space to read the received message */
            if(!ResetLastReceivedSocketAddress(&tempSocketAddress, _socket->m_ipVersion))
            {
                return -1;
            }

            if(_socket->m_ipVersion == IPV4)
            {
                returnedVal = recvfrom(_socket->m_socketID, _bufferToReceiveData, _bufferLengthInBytes, 0, (struct sockaddr*)(tempSocketAddress.m_socketAddressIPV4), &socketAddressSize);
                free(tempSocketAddress.m_socketAddressIPV4);
            }
            else /* IPV6 */
            {
                returnedVal = recvfrom(_socket->m_socketID, _bufferToReceiveData, _bufferLengthInBytes, 0, (struct sockaddr*)(tempSocketAddress.m_socketAddressIPV6), &socketAddressSize);
                free(tempSocketAddress.m_socketAddressIPV6);
            }
        }
    }

    return returnedVal;
}

/* ---------------------------------- End of Main API Functions -------------------------------- */


/* Static Functions: */

static int ValidateNMSocketCreationParams(IpVersion _ipVersion, TransportProtocol _protocol, SocketType _socketType, SocketAddressData _socketData, size_t _waitingConnectionsCap)
{
    size_t ipLength;

    if(_ipVersion != IPV4 && _ipVersion != IPV6)
    {
        return 0; /* Error */
    }

    if(_protocol != UDP && _protocol != TCP)
    {
        return 0; /* Error */
    }

    if(_protocol == TCP && (_waitingConnectionsCap == 0 || _waitingConnectionsCap > MAX_OS_WAITING_SOCKETS_LIMIT))
    {
        return 0; /* Error */
    }

    if(_socketData.m_port < MIN_VALID_PORT_NUM)
    {
        return 0; /* Error */
    }

    if(_socketType != SERVER && _socketType != CLIENT)
    {
        return 0; /* Error */
    }

    return 1; /* OK */
}


static int InitializeSocketAddress(SocketAddress* _socketAddress, IpVersion _ipVersion, const char* _ipAddress, unsigned int _port)
{
    if(_ipVersion == IPV4)
    {
        _socketAddress->m_socketAddressIPV6 = NULL;
        _socketAddress->m_socketAddressIPV4 = malloc(sizeof(struct sockaddr_in));
        if(!_socketAddress->m_socketAddressIPV4)
        {
            return 0;
        }

        memset(_socketAddress->m_socketAddressIPV4, 0, sizeof(*(_socketAddress->m_socketAddressIPV4)));

        _socketAddress->m_socketAddressIPV4->sin_family = AF_INET;
        _socketAddress->m_socketAddressIPV4->sin_port = htons(_port);

        if(!_ipAddress || strcmp(_ipAddress, "any") == 0) /* Indication for any IP address */
        {
            _socketAddress->m_socketAddressIPV4->sin_addr.s_addr = INADDR_ANY;
        }
        else if(strcmp(_ipAddress, "localhost") == 0 || strcmp(_ipAddress, "loopback") == 0) /* loopback */
        {
            _socketAddress->m_socketAddressIPV4->sin_addr.s_addr = INADDR_LOOPBACK;
        }
        else
        {
            _socketAddress->m_socketAddressIPV4->sin_addr.s_addr = inet_addr(_ipAddress);
        }
    }
    else /* IPV6 */
    {
        _socketAddress->m_socketAddressIPV4 = NULL;
        _socketAddress->m_socketAddressIPV6 = malloc(sizeof(struct sockaddr_in6));
        if(!_socketAddress->m_socketAddressIPV6)
        {
            return 0;
        }

        memset(_socketAddress->m_socketAddressIPV6, 0, sizeof(*(_socketAddress->m_socketAddressIPV6)));

        _socketAddress->m_socketAddressIPV6->sin6_port = htons(_port);
        _socketAddress->m_socketAddressIPV6->sin6_family = AF_INET6;

        if(!_ipAddress || strcmp(_ipAddress, "any") == 0) /* Indication for any IP address */
        {
            _socketAddress->m_socketAddressIPV6->sin6_addr = in6addr_any;
        }
        else if(strcmp(_ipAddress, "localhost") == 0 || strcmp(_ipAddress, "loopback") == 0) /* loopback */
        {
            _socketAddress->m_socketAddressIPV6->sin6_addr = in6addr_loopback;
        }
        else
        {
            if(inet_pton(AF_INET6, _ipAddress, &(_socketAddress->m_socketAddressIPV6->sin6_addr)))
            {
                return 0;
            }
        }
    }

    return 1;
}


static int SetServerProperties(NMSocket* _socket, size_t _waitingConnectionsCap)
{
    int optval = 1;

    if(_socket->m_transportProtocol == TCP) /* TCP - more options should be added (reuse port, binding, listen...), in UDP - only binding is needed */
    {
        if(setsockopt(_socket->m_socketID, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) < 0)
        {
            return 0;
        }
    }

    /* Binding - for both UDP and TCP servers */
    if(_socket->m_ipVersion == IPV4)
    {
        if(bind(_socket->m_socketID, (struct sockaddr*)(_socket->m_socketAddress.m_socketAddressIPV4), sizeof(*(_socket->m_socketAddress.m_socketAddressIPV4))) < 0)
        {
            return 0;
        }
    }
    else /* IPV6 */
    {
        if(bind(_socket->m_socketID, (struct sockaddr*)(_socket->m_socketAddress.m_socketAddressIPV6), sizeof(*(_socket->m_socketAddress.m_socketAddressIPV6))) < 0)
        {
            return 0;
        }
    }

    /* Listening - Only for TCP server */
    if(_socket->m_transportProtocol == TCP)
    {
        if(listen(_socket->m_socketID, _waitingConnectionsCap) < 0) /* Sets the socket to be passive */
        {
            return 0;
        }
    }

    return 1;
}


static int ResetLastReceivedSocketAddress(SocketAddress* _lastReceivedSocketAddress, IpVersion _ipVersion)
{
    if(_ipVersion == IPV4)
        {
            if(_lastReceivedSocketAddress->m_socketAddressIPV4) /* Will not be initialized in the first time */
            {
                memset(_lastReceivedSocketAddress->m_socketAddressIPV4, 0, sizeof(*(_lastReceivedSocketAddress->m_socketAddressIPV4))); /* Clear the last received socket address */
            }
            else /* Lazy initialization */
            {
                _lastReceivedSocketAddress->m_socketAddressIPV4 = malloc(sizeof(struct sockaddr_in));
                if(!_lastReceivedSocketAddress->m_socketAddressIPV4)
                {
                    return 0; /* Error */
                }
            }
        }
    else /* IPV6 */
    {
        if(_lastReceivedSocketAddress->m_socketAddressIPV6) /* Will not be initialized in the first time */
        {
            memset(_lastReceivedSocketAddress->m_socketAddressIPV6, 0, sizeof(*(_lastReceivedSocketAddress->m_socketAddressIPV6))); /* Clear the last received socket address */
        }
        else /* Lazy initialization */
        {
            _lastReceivedSocketAddress->m_socketAddressIPV6 = malloc(sizeof(struct sockaddr_in6));
            if(!_lastReceivedSocketAddress->m_socketAddressIPV6)
            {
                return 0; /* Error */
            }
        }
    }

    return 1;
}
