#include "TCPListeningSocket.hpp"
#include <stdexcept> // std::runtime_error
#include <sys/socket.h>
#include <unistd.h> // fcntl
#include <fcntl.h> // fcntl - Special Operation on file descriptors: used to set a Not Blocking flag on sockets
#include <errno.h> // errno
#include "TCPSocket.hpp"


static void ChangeToNoBlockingSocket(int a_socketID) {
    int flags = fcntl(a_socketID, F_GETFL);
    if(flags < 0) {
        throw std::runtime_error("Failed to set the socket as blocking...");
    }

    int statusResult = fcntl(a_socketID, F_SETFL, flags | O_NONBLOCK);
    if(statusResult < 0) {
        throw std::runtime_error("Failed to set the socket as blocking...");
    }
}


nm::TCPListeningSocket::TCPListeningSocket(const unsigned int a_listeningPortNumber, bool a_isNoBlockingRequired)
: TCPSocket("0.0.0.0" , a_listeningPortNumber) // 0.0.0.0 => listening to any ip address
, m_isNoBlockingRequired(a_isNoBlockingRequired)
, m_lastConnectedClientSocketID(-1) {
}


void nm::TCPListeningSocket::Configure() {
    this->SetSocketReuseOption();
    if(this->m_isNoBlockingRequired) {
        this->SetAsNoBlocking();
    }
    this->Bind();
}


bool nm::TCPListeningSocket::Accept() {
    struct sockaddr_in clientSocketAddress;
    unsigned int clientSocketAddressSize = sizeof(clientSocketAddress);
    this->m_lastConnectedClientSocketID = accept(this->GetSocketID(), reinterpret_cast<struct sockaddr*>(&clientSocketAddress), &clientSocketAddressSize);
    if(this->m_lastConnectedClientSocketID < 0)
    {
        if(!this->m_isNoBlockingRequired) {
            throw std::runtime_error("Failed to accept a connection...");
        }

        if(errno != EAGAIN && errno != EWOULDBLOCK) // A real error has occurred
        {
            throw std::runtime_error("Failed to accept a connection...");
        }

        // Else - just a No Blocking signal occurred - continue regularly
        return false;
    }

    if(this->m_isNoBlockingRequired) {
        ChangeToNoBlockingSocket(this->m_lastConnectedClientSocketID);
    }

    return true;
}


void nm::TCPListeningSocket::SetSocketReuseOption() {
    int optionValue = 1;
    int statusResult = setsockopt(this->GetSocketID(), SOL_SOCKET, SO_REUSEADDR, &optionValue, sizeof(optionValue));
    if(statusResult < 0) {
        throw std::runtime_error("Failed to set socket reuse option...");
    }
}


void nm::TCPListeningSocket::SetAsNoBlocking() {
    ChangeToNoBlockingSocket(this->GetSocketID());
}


void nm::TCPListeningSocket::Bind() {
    int statusResult = bind(this->GetSocketID(), reinterpret_cast<struct sockaddr*>(&this->GetSocketAddressData().GetInnerSocketAddress()), sizeof(this->GetSocketAddressData().GetInnerSocketAddress()));
    if(statusResult < 0) {
        throw std::runtime_error("Failed to bind the socket to the port...");
    }
}


void nm::TCPListeningSocket::Listen() {
    int statusResult = listen(this->GetSocketID(), 1); // Listening only to 1 single connecting at a time
    if(statusResult < 0) {
        throw std::runtime_error("Failed to make the socket as listening socket...");
    }
}


nm::TCPSocket::BytesBufferProxy nm::TCPListeningSocket::Receive(const size_t a_bytesToReceive) {
    unsigned char* buffer = new unsigned char[a_bytesToReceive];
    ssize_t receivedBytes = recv(this->m_lastConnectedClientSocketID, static_cast<void*>(buffer), a_bytesToReceive, 0);
    size_t sizeOfBufferToAllocate;

    if(receivedBytes < 0) {
        if(!this->m_isNoBlockingRequired) {
            throw std::runtime_error("Failed to receive a message...");
        }

        if(errno != EAGAIN && errno != EWOULDBLOCK) // A real error has occurred
        {
            throw std::runtime_error("Failed to receive a message...");
        }

        sizeOfBufferToAllocate = 0; // Just no block -> empty buffer (allocation of new[0] is allowed, and MUST be delete[] as well!)
    }
    else {
        sizeOfBufferToAllocate = receivedBytes; // creating the buffer only of {receivedBytes} size
    }

    BytesBufferProxy bufferProxy(buffer, sizeOfBufferToAllocate);
    delete[] buffer;

    return bufferProxy;
}


size_t nm::TCPListeningSocket::Send(const unsigned char* a_message, const size_t a_messageSize) {
    return this->Send(BytesBufferProxy(a_message, a_messageSize));
}


size_t nm::TCPListeningSocket::Send(const BytesBufferProxy &a_message) {
    ssize_t sentBytes = send(this->m_lastConnectedClientSocketID, static_cast<const void*>(a_message.ToBytes()), a_message.Size(), 0);
    if(sentBytes < 0) {
        throw std::runtime_error("Failed to send a message...");
    }

    return size_t(sentBytes);
}