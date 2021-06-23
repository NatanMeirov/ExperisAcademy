#include "TCPSocket.hpp"
#include <cstddef> // size_t
#include <string.h> // memset, memcpy
#include <sys/socket.h> // C standard socket lib
#include <arpa/inet.h> // htons
#include <netinet/in.h> // inet_addr
#include <stdexcept> // std::runtime_error
#include <unistd.h> // close



nm::TCPSocket::TCPSocket::SocketAddressData::SocketAddressData(const std::string& a_ipAddress, const unsigned int a_portNumber)
: m_socketAddress()
, m_ipAddress(a_ipAddress)
, m_portNumber(a_portNumber) {
    memset(&this->m_socketAddress, 0, sizeof(this->m_socketAddress));
    this->m_socketAddress.sin_addr.s_addr = inet_addr(this->m_ipAddress.c_str());
    this->m_socketAddress.sin_port = htons(this->m_portNumber);
    this->m_socketAddress.sin_family = AF_INET; // IPv4
}


nm::TCPSocket::TCPSocket(const std::string& a_ipAddress, const unsigned int a_portNumber)
: m_socketAddressData(a_ipAddress, a_portNumber)
, m_socketID(socket(AF_INET, SOCK_STREAM, 0)) {
    if(this->m_socketID < 0) {
        throw std::runtime_error("Failed to create a socket...");
    }
}


nm::TCPSocket::~TCPSocket() {
    close(this->m_socketID);
}


static unsigned char* CreateBytesBufferFromOtherBuffer(const unsigned char* a_bytesBufferToCopy, const size_t a_sizeOfBuffer) {
    unsigned char* newBuffer = new unsigned char[a_sizeOfBuffer];
    memcpy(newBuffer, a_bytesBufferToCopy, a_sizeOfBuffer);

    return newBuffer;
}


nm::TCPSocket::BytesBufferProxy::BytesBufferProxy()
: m_bufferOfBytes(new unsigned char[0])
, m_bufferSize(0) {
}


nm::TCPSocket::BytesBufferProxy::BytesBufferProxy(const unsigned char*  a_bufferOfBytes, const size_t a_bufferSize)
: m_bufferOfBytes(CreateBytesBufferFromOtherBuffer(a_bufferOfBytes, a_bufferSize))
, m_bufferSize(a_bufferSize) {
}


nm::TCPSocket::BytesBufferProxy::BytesBufferProxy(const BytesBufferProxy& a_other)
: m_bufferOfBytes(CreateBytesBufferFromOtherBuffer(a_other.m_bufferOfBytes, a_other.m_bufferSize))
, m_bufferSize(a_other.m_bufferSize) {
}


nm::TCPSocket::BytesBufferProxy& nm::TCPSocket::BytesBufferProxy::operator=(const BytesBufferProxy& a_other) {
    if(this != &a_other) { // Saving from self copy-assignment
        unsigned char* newBuffer = CreateBytesBufferFromOtherBuffer(a_other.m_bufferOfBytes, a_other.m_bufferSize); // Saving for a possible thrown exception - even if it would occur, the buffer;s pointer will not be a dangling pointer
        delete[] this->m_bufferOfBytes;

        this->m_bufferOfBytes = newBuffer;
        this->m_bufferSize = a_other.m_bufferSize;
    }

    return *this;
}


nm::TCPSocket::BytesBufferProxy::BytesBufferProxy(BytesBufferProxy&& a_rvalue) noexcept
: m_bufferOfBytes(a_rvalue.m_bufferOfBytes)
, m_bufferSize(a_rvalue.m_bufferSize) {
    a_rvalue.m_bufferOfBytes = nullptr; // MUST to do in order to save from a delete[]
}


nm::TCPSocket::BytesBufferProxy& nm::TCPSocket::BytesBufferProxy::operator=(BytesBufferProxy&& a_rvalue) noexcept {
    delete[] this->m_bufferOfBytes;

    this->m_bufferOfBytes = a_rvalue.m_bufferOfBytes;
    this->m_bufferSize = a_rvalue.m_bufferSize;
    a_rvalue.m_bufferOfBytes = nullptr; // MUST to do in order to save from a delete[]

    return *this;
}


nm::TCPSocket::BytesBufferProxy::~BytesBufferProxy() {
    delete[] this->m_bufferOfBytes;
}


nm::TCPSocket::BytesBufferProxy& nm::TCPSocket::BytesBufferProxy::operator+=(const BytesBufferProxy& a_other) {
    size_t newBufferSize = this->m_bufferSize + a_other.m_bufferSize;
    unsigned char* newBuffer = new unsigned char[newBufferSize];

    memcpy(newBuffer, this->m_bufferOfBytes, this->m_bufferSize);
    memcpy(newBuffer + this->m_bufferSize, a_other.m_bufferOfBytes, a_other.m_bufferSize);

    delete[] this->m_bufferOfBytes;
    this->m_bufferOfBytes = newBuffer;
    this->m_bufferSize = newBufferSize;

    return *this;
}


nm::TCPSocket::BytesBufferProxy nm::TCPSocket::BytesBufferProxy::operator+(const BytesBufferProxy& a_other) const {
    BytesBufferProxy newBytesBuffer(*this);

    return newBytesBuffer += a_other;
}


void nm::TCPSocket::Connect() {
    int statusResult = connect(this->m_socketID, reinterpret_cast<struct sockaddr*>(&this->m_socketAddressData.GetInnerSocketAddress()), sizeof(this->m_socketAddressData.GetInnerSocketAddress()));
    if(statusResult < 0) {
        throw std::runtime_error("Failed to connect the socket to the other socket address...");
    }
}


size_t nm::TCPSocket::Send(const unsigned char* a_message, const size_t a_messageSize) {
    return this->Send(BytesBufferProxy(a_message, a_messageSize));
}


size_t nm::TCPSocket::Send(const BytesBufferProxy &a_message) {
    ssize_t sentBytes = send(this->m_socketID, static_cast<const void*>(a_message.ToBytes()), a_message.Size(), 0);
    if(sentBytes < 0) {
        throw std::runtime_error("Failed to send a message...");
    }

    return size_t(sentBytes);
}


nm::TCPSocket::BytesBufferProxy nm::TCPSocket::Receive(const size_t a_bytesToReceive) {
    unsigned char* buffer = new unsigned char[a_bytesToReceive];
    ssize_t receivedBytes = recv(this->m_socketID, static_cast<void*>(buffer), a_bytesToReceive, 0);
    if(receivedBytes < 0) {
        delete[] buffer;
        throw std::runtime_error("Failed to receive a message...");
    }

    BytesBufferProxy bufferProxy(buffer, receivedBytes);
    delete[] buffer;

    return bufferProxy;
}