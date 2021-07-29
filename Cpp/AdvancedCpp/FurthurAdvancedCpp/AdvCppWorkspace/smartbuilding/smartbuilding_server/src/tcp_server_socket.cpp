#include "tcp_server_socket.hpp"
#include "tcp_listening_socket.hpp"


infra::TCPServerSocket::TCPServerSocket(unsigned int a_listeningPortNumber, bool a_isNoBlockingRequired)
: TCPListeningSocket(a_listeningPortNumber, a_isNoBlockingRequired)
, m_clientIDToSendMessageTo(-1)
, m_clientIDToReceiveMessageFrom(-1)
{
}
