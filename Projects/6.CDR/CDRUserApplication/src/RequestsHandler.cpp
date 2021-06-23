#include "../inc/RequestsHandler.hpp"
#include "../../Infrastructure/JsonSerializer/json.hpp"


nm::cdr::RequestsHandler::RequestsHandler(const std::string& a_ipAddressOfServer, const unsigned int a_portNumberOfServer)
: m_connectionSocket(a_ipAddressOfServer, a_portNumberOfServer) {
    this->m_connectionSocket.Connect();
}


void nm::cdr::RequestsHandler::Request(const std::string& a_request) {
    this->m_connectionSocket.Send(reinterpret_cast<const unsigned char*>(a_request.c_str()), (a_request.length() + 1)); // +1 stands for '\0'
}


nlohmann::json nm::cdr::RequestsHandler::Response() {
    nm::TCPSocket::BytesBufferProxy receivedBuffer = this->m_connectionSocket.Receive(RequestsHandler::RESPONSE_MAX_BUFFER_SIZE);
    return nlohmann::json(reinterpret_cast<const char*>(receivedBuffer.ToBytes()));
}