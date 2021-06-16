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
    void Listen(); // A derived class (optional: TCPListeningServerSocket) - can overload Listen to listen to more than 1 client at a time (and should maintain a list of clients as well, and not only the last connected client)
    bool Accept(); // true if new connection has arrived (more useful for No Blocking listening socket), else false [WARNING: if socket is set to support No Blocking option, the Accept would return false. If you use Receive while NO new connections has arrived - an exception will be thrown (error of receiving a message)]
    virtual BytesBufferProxy Receive(const size_t a_bytesToReceive) override; // Returns the received buffer (or 0 if no blocking and didn't receive anything), Throws on failure
    virtual size_t Send(const unsigned char* a_message, const size_t a_messageSize) override; // Retuns the number of sent bytes, Throws on failure
    virtual size_t Send(const BytesBufferProxy& a_message) override; // Retuns the number of sent bytes, Throws on failure

protected:
    SocketID& GetLastConnectedClientSocketID() { return this->m_lastConnectedClientSocketID; }

private:
    void SetSocketReuseOption();
    void SetAsNoBlocking();
    void Bind();

    bool m_isNoBlockingRequired;
    SocketID m_lastConnectedClientSocketID;
};

} // nm


#endif // __NM_TCPSERVERSOCKET_HPP__