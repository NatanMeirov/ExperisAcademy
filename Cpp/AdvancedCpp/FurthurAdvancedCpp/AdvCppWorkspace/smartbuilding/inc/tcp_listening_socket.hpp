#ifndef NM_TCP_LISTENING_SOCKET_HPP
#define NM_TCP_LISTENING_SOCKET_HPP


#include <cstddef> // size_t
#include <utility> // std::pair
#include "tcp_socket.hpp"


namespace infra
{

class TCPListeningSocket : public TCPSocket
{
public:
    TCPListeningSocket(unsigned int a_listeningPortNumber, bool a_isNoBlockingRequired = false); // The listening socket will listen to any Ip Address by default
    TCPListeningSocket(const TCPListeningSocket& a_other) = delete;
    TCPListeningSocket& operator=(const TCPListeningSocket& a_other) = delete;
    virtual ~TCPListeningSocket() = default;

    void Listen(unsigned int a_connectionsWaitingQueueSize);
    bool Accept(); // true if new connection has arrived (more useful for No Blocking listening socket), else false [WARNING: if socket is set to support No Blocking option, the Accept would return false. If you use Receive while NO new connections has arrived - an exception will be thrown (error of receiving a message)]
    virtual BytesBufferProxy Receive(size_t a_bytesToReceive) override; // Returns the received buffer (or 0 if no blocking and didn't receive anything), Throws on failure [Receives from the last accepted client]
    SocketID GetLastAcceptedClientSocketID() const { return m_lastAcceptedClientSocketID; }
    std::pair<std::string,unsigned int> GetLastAcceptedClientIpAndPortData() const { return m_lastAcceptedClientSocketAddressData.GetIpAndPort(); }

protected:
    virtual SocketID GetSocketIDToSendTheMessageTo() const override { return m_lastAcceptedClientSocketID; }
    virtual SocketID GetSocketIDToReceiveTheMessageFrom() const { return m_lastAcceptedClientSocketID; }

private:
    void SetSocketReuseOption();
    void SetAsNoBlocking();
    void Bind();
    void Configure();

private:
    SocketID m_lastAcceptedClientSocketID;
    SocketAddressData m_lastAcceptedClientSocketAddressData;
    bool m_isNoBlockingRequired;
};

} // infra


#endif // NM_TCP_LISTENING_SOCKET_HPP