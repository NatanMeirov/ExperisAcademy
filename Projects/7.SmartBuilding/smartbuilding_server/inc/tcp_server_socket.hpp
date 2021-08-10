#ifndef NM_TCP_SERVER_SOCKET_HPP
#define NM_TCP_SERVER_SOCKET_HPP


#include "tcp_listening_socket.hpp"


namespace infra
{

class TCPServerSocket : public TCPListeningSocket
{
public:
    TCPServerSocket(unsigned int a_listeningPortNumber, bool a_isNoBlockingRequired = false); // The tcp server socket will listen to any Ip Address by default
    TCPServerSocket(const TCPServerSocket& a_other) = delete;
    TCPServerSocket& operator=(const TCPServerSocket& a_other) = delete;
    virtual ~TCPServerSocket() = default;

    // An addition to the listening socket - enables to handle multiple clients at the same time (and not only the last accepted client!)
    void SetClientIDToSendMessageTo(SocketID a_clientID) { m_clientIDToSendMessageTo = a_clientID; }
    void SetClientIDToReceiveMessageFrom(SocketID a_clientID) { m_clientIDToReceiveMessageFrom = a_clientID; }
    SocketID InnerSocketID() const { return GetSelfSocketID(); }

protected:
    virtual SocketID GetSocketIDToSendTheMessageTo() const override { return m_clientIDToSendMessageTo; }
    virtual SocketID GetSocketIDToReceiveTheMessageFrom() const override { return m_clientIDToReceiveMessageFrom; }

private:
    SocketID m_clientIDToSendMessageTo;
    SocketID m_clientIDToReceiveMessageFrom;
};

} // infra


#endif // NM_TCP_SERVER_SOCKET_HPP
