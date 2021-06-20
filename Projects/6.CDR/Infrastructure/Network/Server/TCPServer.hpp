#ifndef __NM_TCPSERVER_HPP__
#define __NM_TCPSERVER_HPP__

#include <cstddef> // size_t
#include "C-API_GenericServer/GenericServer-IPv4.h"


namespace nm {

class TCPServer {
public:
    TCPServer(void* _applicationInfo, ClientMessageHandler _onClientMessage, ErrorHandler _onError, NewClientConnectionHandler _onNewClientConnection, CloseClientConnectionHandler _onCloseClientConnection, unsigned int _serverPort, size_t _maxWaitingConnections, size_t _receivedMessagesBufferSize);
    TCPServer(const TCPServer& a_other) = delete;
    TCPServer& operator=(const TCPServer& a_other) = delete;
    ~TCPServer();

    void Run();

private:
    Server* m_server;
};

} // nm


#endif // __NM_TCPSERVER_HPP__