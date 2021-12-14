#ifndef NM_TCP_SOCKET_HPP
#define NM_TCP_SOCKET_HPP


#include <cstddef> // size_t
#include <string> // std::string
#include <ostream> // std::ostream
#include <netinet/in.h> // struct sockaddr_in
#include <string.h> // memset
#include <utility> // std::pair, std::make_pair


namespace infra
{

class TCPSocket
{
protected:
    typedef int SocketID;

    class SocketAddressData
    {
    public:
        SocketAddressData() : m_socketAddress(), m_ipAddress(), m_portNumber() { memset(&m_socketAddress, 0, sizeof(m_socketAddress)); }
        SocketAddressData(const std::string& a_ipAddress, unsigned int a_portNumber);

        struct sockaddr_in& GetInnerSocketAddress() { return m_socketAddress; }
        size_t GetInnerSocketAddressSize() const { return sizeof(m_socketAddress); }
        std::pair<std::string,unsigned int> GetIpAndPort() const { return std::make_pair(m_ipAddress, m_portNumber); }

    private:
        struct sockaddr_in m_socketAddress;
        std::string m_ipAddress;
        unsigned int m_portNumber;
    };

public:
    class BytesBufferProxy // For RAII
    {
        friend std::ostream& operator<<(std::ostream& a_os, const BytesBufferProxy& a_bufferProxy) { return a_os << a_bufferProxy.m_bufferOfBytes; }
    public:
        BytesBufferProxy();
        BytesBufferProxy(const unsigned char*  a_bufferOfBytes, size_t a_bufferSize);
        BytesBufferProxy(const BytesBufferProxy& a_other);
        BytesBufferProxy& operator=(const BytesBufferProxy& a_other);
        BytesBufferProxy(BytesBufferProxy&& a_rvalue) noexcept; // Move copy c'tor (better performance)
        BytesBufferProxy& operator=(BytesBufferProxy&& a_rvalue) noexcept; // Move copy-assignment (better performance)
        ~BytesBufferProxy();

        BytesBufferProxy& operator+=(const BytesBufferProxy& a_other);
        BytesBufferProxy operator+(const BytesBufferProxy& a_other) const;

        const unsigned char* ToBytes() const { return m_bufferOfBytes; };
        size_t Size() const { return m_bufferSize; }

    private:
        unsigned char* m_bufferOfBytes;
        size_t m_bufferSize;
    };

    TCPSocket(const std::string& a_ipAddress, unsigned int a_portNumber); // Validation of an IP addresses is a user responsability!
    TCPSocket(SocketID a_fileDescriptorSocket); // Validation of a valid file descriptor number is a user responsability! To be able to use Connect() method of the new TCPSocket wrapper, make sure that the created TCPSocket of FD, is a FD of a tcp socket!
    TCPSocket(const TCPSocket& a_other) = delete;
    TCPSocket& operator=(const TCPSocket& a_other) = delete;
    virtual ~TCPSocket();

    std::pair<std::string,unsigned int> GetIpAndPortData() const { return m_socketAddressData.GetIpAndPort(); }
    void Connect(); // Throws on failure
    virtual size_t Send(const unsigned char* a_message, size_t a_messageSize, bool a_provideFullMessageSending = true); // Retuns the number of sent bytes, Throws on failure
    virtual size_t Send(const BytesBufferProxy& a_message, bool a_provideFullMessageSending = true); // Returns the number of sent bytes, Throws on failure
    virtual BytesBufferProxy Receive(size_t a_bytesToReceive); // Returns the received buffer, Throws on failure

protected:
    SocketAddressData& GetSelfSocketAddressData() { return m_socketAddressData; }
    SocketID GetSelfSocketID() const { return m_socketID; }
    virtual SocketID GetSocketIDToSendTheMessageTo() const { return m_socketID; }

private:
    static SocketAddressData CreateSocketAddressDataFromFileDescriptorSocket(SocketID a_fileDescriptorSocket);

private:
    SocketAddressData m_socketAddressData;
    SocketID m_socketID;
};

} // infra


#endif // NM_TCP_SOCKET_HPP
