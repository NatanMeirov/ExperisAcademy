#include "TCPServer.hpp"
#include <cstddef> // size_t
#include "C-API_GenericServer/GenericServer-IPv4.h"
#include <stdexcept> // std::runtime_error


nm::cdr::TCPServer::TCPServer(void* a_applicationInfo, ClientMessageHandler a_onClientMessage, ErrorHandler a_onError, NewClientConnectionHandler a_onNewClientConnection, CloseClientConnectionHandler a_onCloseClientConnection, unsigned int a_serverPort, size_t a_maxWaitingConnections, size_t a_receivedMessagesBufferSize)
: m_server(ServerCreate(a_applicationInfo, a_onClientMessage, a_onError, a_onNewClientConnection, a_onCloseClientConnection, a_serverPort, a_maxWaitingConnections, a_receivedMessagesBufferSize)) {
    if(!this->m_server) {
        throw std::runtime_error("Failed to initialize the server...");
    }
}


nm::cdr::TCPServer::~TCPServer() {
    ServerDestroy(&this->m_server);
}


void nm::cdr::TCPServer::Run() {
    ServerRun(this->m_server);
}