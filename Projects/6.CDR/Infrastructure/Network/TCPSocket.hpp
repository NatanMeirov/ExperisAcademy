#ifndef __NM_TCPSOCKET_HPP__
#define __NM_TCPSOCKET_HPP__


#include <cstddef> // size_t
#include <string> // std::string
#include <ostream> // std::ostream
#include <netinet/in.h> // struct sockaddr_in


namespace nm {

class TCPSocket {
protected:
    typedef int SocketID;

    class SocketAddressData {
    public:
        SocketAddressData(const std::string& a_ipAddress, const unsigned int a_portNumber);

        struct sockaddr_in& GetInnerSocketAddress() { return this->m_socketAddress; }
        size_t GetInnerSocketAddressSize() const { return sizeof(this->m_socketAddress); }

    private:
        struct sockaddr_in m_socketAddress;
        std::string m_ipAddress;
        unsigned int m_portNumber;
    };

public:
    class BytesBufferProxy { // For RAII
        friend std::ostream& operator<<(std::ostream& a_os, const BytesBufferProxy& a_bufferProxy) { return a_os << a_bufferProxy.m_bufferOfBytes; }
    public:
        BytesBufferProxy();
        BytesBufferProxy(const unsigned char*  a_bufferOfBytes, const size_t a_bufferSize);
        BytesBufferProxy(const BytesBufferProxy& a_other);
        BytesBufferProxy& operator=(const BytesBufferProxy& a_other);
        BytesBufferProxy(BytesBufferProxy&& a_rvalue) noexcept; // Move copy c'tor (better performance)
        BytesBufferProxy& operator=(BytesBufferProxy&& a_rvalue) noexcept; // Move copy-assignment (better performance)
        ~BytesBufferProxy();

        BytesBufferProxy& operator+=(const BytesBufferProxy& a_other);
        BytesBufferProxy operator+(const BytesBufferProxy& a_other) const;

        const unsigned char* ToBytes() const { return this->m_bufferOfBytes; };
        size_t Size() const { return this->m_bufferSize; }

    private:
        unsigned char* m_bufferOfBytes;
        size_t m_bufferSize;
    };

    TCPSocket(const std::string& a_ipAddress, const unsigned int a_portNumber); // Validation of an IP addresses is a user responsability!
    TCPSocket(const TCPSocket& a_other) = delete;
    TCPSocket& operator=(const TCPSocket& a_other) = delete;
    virtual ~TCPSocket();

    void Connect(); // Throws on failure
    virtual size_t Send(const unsigned char* a_message, const size_t a_messageSize); // Retuns the number of sent bytes, Throws on failure
    virtual size_t Send(const BytesBufferProxy& a_message); // Retuns the number of sent bytes, Throws on failure
    virtual BytesBufferProxy Receive(const size_t a_bytesToReceive); // Returns the received buffer, Throws on failure

protected:
    SocketAddressData& GetSocketAddressData() { return this->m_socketAddressData; }
    SocketID& GetSocketID() { return this->m_socketID; }

private:
    SocketAddressData m_socketAddressData;
    SocketID m_socketID;
};

} // nm


#endif // __NM_TCPSOCKET_HPP__