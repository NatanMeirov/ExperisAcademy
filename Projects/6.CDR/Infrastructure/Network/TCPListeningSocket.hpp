#ifndef __NM_TCPLISTENINGSOCKET_HPP__
#define __NM_TCPLISTENINGSOCKET_HPP__


#include <cstddef> // size_t
#include "TCPSocket.hpp"


namespace nm {

class TCPListeningSocket : public TCPSocket {
public:
    TCPListeningSocket(const unsigned int a_listeningPortNumber, bool a_isNoBlockingRequired = false); // The listening socket will listen to any Ip Address by default
    TCPListeningSocket(const TCPListeningSocket& a_other) = delete;
    TCPListeningSocket& operator=(const TCPListeningSocket& a_other) = delete;
    virtual ~TCPListeningSocket() = default;

    void Configure(); // Throws on failure
    void Listen(const unsigned int a_connectionsWaitingQueueSize);
    bool Accept(); // true if new connection has arrived (more useful for No Blocking listening socket), else false [WARNING: if socket is set to support No Blocking option, the Accept would return false. If you use Receive while NO new connections has arrived - an exception will be thrown (error of receiving a message)]
    virtual BytesBufferProxy Receive(const size_t a_bytesToReceive) override; // Returns the received buffer (or 0 if no blocking and didn't receive anything), Throws on failure [Receives from the last accepted client]

protected:
    SocketID GetLastAcceptedClientSocketID() { return this->m_lastAcceptedClientSocketID; }
    virtual SocketID GetSocketIDToSendTheMessageTo() override { return this->GetLastAcceptedClientSocketID(); }

private:
    void SetSocketReuseOption();
    void SetAsNoBlocking();
    void Bind();

    bool m_isNoBlockingRequired;
    SocketID m_lastAcceptedClientSocketID;
};

} // nm


#endif // __NM_TCPSERVERSOCKET_HPP__