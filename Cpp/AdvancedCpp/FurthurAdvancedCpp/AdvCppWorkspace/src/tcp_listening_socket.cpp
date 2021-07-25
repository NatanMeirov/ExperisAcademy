#include "tcp_listening_socket.hpp"
#include <arpa/inet.h> // inet_ntoa, ntohs
#include <stdexcept> // std::runtime_error
#include <sys/socket.h>
#include <unistd.h> // fcntl
#include <fcntl.h> // fcntl - Special Operation on file descriptors: used to set a Not Blocking flag on sockets
#include <errno.h> // errno
#include "tcp_socket.hpp"


static void ChangeToNoBlockingSocket(int a_socketID)
{
    int flags = fcntl(a_socketID, F_GETFL);
    if(flags < 0)
    {
        throw std::runtime_error("Failed to set the socket as blocking...");
    }

    int statusResult = fcntl(a_socketID, F_SETFL, flags | O_NONBLOCK);
    if(statusResult < 0)
    {
        throw std::runtime_error("Failed to set the socket as blocking...");
    }
}


infra::TCPListeningSocket::TCPListeningSocket(unsigned int a_listeningPortNumber, bool a_isNoBlockingRequired)
: TCPSocket("0.0.0.0" , a_listeningPortNumber) // 0.0.0.0 => listening to any ip address
, m_lastAcceptedClientSocketID(-1)
, m_lastAcceptedClientSocketAddressData()
, m_isNoBlockingRequired(a_isNoBlockingRequired)
{
}


void infra::TCPListeningSocket::Configure()
{
    SetSocketReuseOption();
    if(m_isNoBlockingRequired)
    {
        SetAsNoBlocking();
    }
    Bind();
}


bool infra::TCPListeningSocket::Accept()
{
    struct sockaddr_in clientSocketAddress;
    unsigned int clientSocketAddressSize = sizeof(clientSocketAddress);
    m_lastAcceptedClientSocketID = accept(GetSocketID(), reinterpret_cast<struct sockaddr*>(&clientSocketAddress), &clientSocketAddressSize);
    if(m_lastAcceptedClientSocketID < 0)
    {
        if(!m_isNoBlockingRequired)
        {
            throw std::runtime_error("Failed to accept a connection...");
        }

        if(errno != EAGAIN && errno != EWOULDBLOCK) // A real error has occurred
        {
            throw std::runtime_error("Failed to accept a connection...");
        }

        // Else - just a No Blocking signal occurred - continue regularly
        return false;
    }

    m_lastAcceptedClientSocketAddressData = TCPSocket::SocketAddressData(std::string(inet_ntoa(clientSocketAddress.sin_addr)), ntohs(clientSocketAddress.sin_port));

    if(m_isNoBlockingRequired)
    {
        ChangeToNoBlockingSocket(m_lastAcceptedClientSocketID);
    }

    return true;
}


void infra::TCPListeningSocket::SetSocketReuseOption()
{
    int optionValue = 1;
    int statusResult = setsockopt(GetSocketID(), SOL_SOCKET, SO_REUSEADDR, &optionValue, sizeof(optionValue));
    if(statusResult < 0)
    {
        throw std::runtime_error("Failed to set socket reuse option...");
    }
}


void infra::TCPListeningSocket::SetAsNoBlocking()
{
    ChangeToNoBlockingSocket(GetSocketID());
}


void infra::TCPListeningSocket::Bind()
{
    int statusResult = bind(GetSocketID(), reinterpret_cast<struct sockaddr*>(&GetSocketAddressData().GetInnerSocketAddress()), sizeof(GetSocketAddressData().GetInnerSocketAddress()));
    if(statusResult < 0)
    {
        throw std::runtime_error("Failed to bind the socket to the port...");
    }
}


void infra::TCPListeningSocket::Listen(unsigned int a_connectionsWaitingQueueSize)
{
    int statusResult = listen(GetSocketID(), a_connectionsWaitingQueueSize);
    if(statusResult < 0)
    {
        throw std::runtime_error("Failed to make the socket as listening socket...");
    }
}


infra::TCPSocket::BytesBufferProxy infra::TCPListeningSocket::Receive(size_t a_bytesToReceive)
{
    unsigned char* buffer = new unsigned char[a_bytesToReceive];
    size_t receivedBytes = recv(GetSocketIDToReceiveTheMessageFrom(), static_cast<void*>(buffer), a_bytesToReceive, 0);
    size_t sizeOfBufferToAllocate;

    if(receivedBytes == size_t(-1)) // Representation of max size_t value
    {
        if(!m_isNoBlockingRequired)
        {
            throw std::runtime_error("Failed to receive a message...");
        }

        if(errno != EAGAIN && errno != EWOULDBLOCK) // A real error has occurred
        {
            throw std::runtime_error("Failed to receive a message...");
        }

        sizeOfBufferToAllocate = 0; // Just no block -> empty buffer (allocation of new[0] is allowed, and MUST be delete[] as well!)
    }
    else
    {
        sizeOfBufferToAllocate = receivedBytes; // creating the buffer only of {receivedBytes} size
    }

    BytesBufferProxy bufferProxy(buffer, sizeOfBufferToAllocate);
    delete[] buffer;

    return bufferProxy;
}