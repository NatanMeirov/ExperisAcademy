#include "tcp_socket.hpp"
#include <cstddef> // size_t
#include <string> // std::string, std::to_string
#include <string.h> // memset, memcpy
#include <sys/socket.h> // C standard socket lib
#include <arpa/inet.h> // htons, ntohs
#include <netinet/in.h> // inet_addr, inet_ntoa
#include <stdexcept> // std::runtime_error
#include <unistd.h> // close


infra::TCPSocket::TCPSocket::SocketAddressData infra::TCPSocket::CreateSocketAddressDataFromFileDescriptorSocket(SocketID a_fileDescriptorSocket)
{
    struct sockaddr_in socketAddress;
    unsigned int length = sizeof(socketAddress);
    if(getpeername(a_fileDescriptorSocket, reinterpret_cast<struct sockaddr*>(&socketAddress), &length) < 0)
    {
        throw std::runtime_error("Failed to create a socket from File Descriptor Socket: { " + std::to_string(a_fileDescriptorSocket) + " }");
    }


    return SocketAddressData(std::string(inet_ntoa(socketAddress.sin_addr)), ntohs(socketAddress.sin_port));
}


infra::TCPSocket::TCPSocket::SocketAddressData::SocketAddressData(const std::string& a_ipAddress, unsigned int a_portNumber)
: m_socketAddress()
, m_ipAddress(a_ipAddress)
, m_portNumber(a_portNumber)
{
    memset(&m_socketAddress, 0, sizeof(m_socketAddress));
    m_socketAddress.sin_addr.s_addr = inet_addr(m_ipAddress.c_str());
    m_socketAddress.sin_port = htons(m_portNumber);
    m_socketAddress.sin_family = AF_INET; // IPv4
}


infra::TCPSocket::TCPSocket(const std::string& a_ipAddress, unsigned int a_portNumber)
: m_socketAddressData(a_ipAddress, a_portNumber)
, m_socketID(socket(AF_INET, SOCK_STREAM, 0))
{
    if(m_socketID < 0)
    {
        throw std::runtime_error("Failed to create a socket to { Ip: " + a_ipAddress + ", Port: " + std::to_string(a_portNumber) + " }");
    }
}


infra::TCPSocket::TCPSocket(SocketID a_fileDescriptorSocket)
: m_socketAddressData(CreateSocketAddressDataFromFileDescriptorSocket(a_fileDescriptorSocket))
, m_socketID(a_fileDescriptorSocket)
{
}


infra::TCPSocket::~TCPSocket()
{
    close(m_socketID);
}


static unsigned char* CreateBytesBufferFromOtherBuffer(const unsigned char* a_bytesBufferToCopy, size_t a_sizeOfBuffer)
{
    unsigned char* newBuffer = new unsigned char[a_sizeOfBuffer];
    memcpy(newBuffer, a_bytesBufferToCopy, a_sizeOfBuffer);

    return newBuffer;
}


infra::TCPSocket::BytesBufferProxy::BytesBufferProxy()
: m_bufferOfBytes(new unsigned char[0])
, m_bufferSize(0)
{
}


infra::TCPSocket::BytesBufferProxy::BytesBufferProxy(const unsigned char*  a_bufferOfBytes, size_t a_bufferSize)
: m_bufferOfBytes(CreateBytesBufferFromOtherBuffer(a_bufferOfBytes, a_bufferSize))
, m_bufferSize(a_bufferSize)
{
}


infra::TCPSocket::BytesBufferProxy::BytesBufferProxy(const BytesBufferProxy& a_other)
: m_bufferOfBytes(CreateBytesBufferFromOtherBuffer(a_other.m_bufferOfBytes, a_other.m_bufferSize))
, m_bufferSize(a_other.m_bufferSize)
{
}


infra::TCPSocket::BytesBufferProxy& infra::TCPSocket::BytesBufferProxy::operator=(const BytesBufferProxy& a_other) {
    if(this != &a_other) // Saving from self copy-assignable
    {
        unsigned char* newBuffer = CreateBytesBufferFromOtherBuffer(a_other.m_bufferOfBytes, a_other.m_bufferSize); // Saving for a possible thrown exception - even if it would occur, the buffer;s pointer will not be a dangling pointer
        delete[] m_bufferOfBytes;

        m_bufferOfBytes = newBuffer;
        m_bufferSize = a_other.m_bufferSize;
    }

    return *this;
}


infra::TCPSocket::BytesBufferProxy::BytesBufferProxy(BytesBufferProxy&& a_rvalue) noexcept
: m_bufferOfBytes(a_rvalue.m_bufferOfBytes)
, m_bufferSize(a_rvalue.m_bufferSize)
{
    a_rvalue.m_bufferOfBytes = nullptr; // MUST to do in order to save from a delete[]
}


infra::TCPSocket::BytesBufferProxy& infra::TCPSocket::BytesBufferProxy::operator=(BytesBufferProxy&& a_rvalue) noexcept
{
    delete[] m_bufferOfBytes;

    m_bufferOfBytes = a_rvalue.m_bufferOfBytes;
    m_bufferSize = a_rvalue.m_bufferSize;
    a_rvalue.m_bufferOfBytes = nullptr; // MUST to do in order to save from a delete[]

    return *this;
}


infra::TCPSocket::BytesBufferProxy::~BytesBufferProxy()
{
    delete[] m_bufferOfBytes;
}


infra::TCPSocket::BytesBufferProxy& infra::TCPSocket::BytesBufferProxy::operator+=(const BytesBufferProxy& a_other)
{
    size_t newBufferSize = m_bufferSize + a_other.m_bufferSize;
    unsigned char* newBuffer = new unsigned char[newBufferSize];

    memcpy(newBuffer, m_bufferOfBytes, m_bufferSize);
    memcpy(newBuffer + m_bufferSize, a_other.m_bufferOfBytes, a_other.m_bufferSize);

    delete[] m_bufferOfBytes;
    m_bufferOfBytes = newBuffer;
    m_bufferSize = newBufferSize;

    return *this;
}


infra::TCPSocket::BytesBufferProxy infra::TCPSocket::BytesBufferProxy::operator+(const BytesBufferProxy& a_other) const
{
    BytesBufferProxy newBytesBuffer(*this);

    return newBytesBuffer += a_other;
}


void infra::TCPSocket::Connect()
{
    int statusResult = connect(m_socketID, reinterpret_cast<struct sockaddr*>(&m_socketAddressData.GetInnerSocketAddress()), sizeof(m_socketAddressData.GetInnerSocketAddress()));
    if(statusResult < 0)
    {
        throw std::runtime_error("Failed to connect the socket to the other socket address...");
    }
}


size_t infra::TCPSocket::Send(const unsigned char* a_message, size_t a_messageSize, bool a_provideFullMessageSending)
{
    return Send(BytesBufferProxy(a_message, a_messageSize), a_provideFullMessageSending);
}


size_t infra::TCPSocket::Send(const BytesBufferProxy& a_message, bool a_provideFullMessageSending)
{
    size_t totalSentBytes = send(GetSocketIDToSendTheMessageTo(), static_cast<const void*>(a_message.ToBytes()), a_message.Size(), 0);
    if(totalSentBytes == size_t(-1)) // Representation of max size_t value
    {
        throw std::runtime_error("Failed to send a message...");
    }

    if(a_provideFullMessageSending && totalSentBytes < a_message.Size()) // totalSentBytes cannot be size_t(-1) [max size_t value]
    {
        while(totalSentBytes < a_message.Size())
        {
            size_t newBytesSent = send(GetSocketIDToSendTheMessageTo(), static_cast<const void*>(a_message.ToBytes() + totalSentBytes), a_message.Size() - totalSentBytes, 0);
            if(newBytesSent == size_t(-1)) // An internal failure while tried to send the rest of the message (the connection could have lost - cannot provide full message sending in that case...)
            {
                break;
            }
            totalSentBytes += newBytesSent;
        }
    }

    return totalSentBytes;
}


infra::TCPSocket::BytesBufferProxy infra::TCPSocket::Receive(size_t a_bytesToReceive)
{
    unsigned char* buffer = new unsigned char[a_bytesToReceive];
    size_t receivedBytes = recv(m_socketID, static_cast<void*>(buffer), a_bytesToReceive, 0);
    if(receivedBytes == size_t(-1)) // Representation of max size_t value
    {
        delete[] buffer;
        throw std::runtime_error("Failed to receive a message...");
    }

    BytesBufferProxy bufferProxy(buffer, receivedBytes);
    delete[] buffer;

    return bufferProxy;
}
