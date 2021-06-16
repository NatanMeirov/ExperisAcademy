#ifndef USE_SERVER_LOGGER
#define USE_SERVER_LOGGER

#include "../Server/TCPServer.hpp"
#include <iostream>


namespace nm {

namespace test {

struct TestTCPServer {
    TestTCPServer(int a_counter = 0) : m_counter(a_counter) {}
    int m_counter;
};

void OnMessage(void* _message, int _clientID, Response* _response, void* _applicationInfo) {
    std::cout << "Message from client "  << _clientID << ": " << (const char*)_message << std::endl;
    _response->m_responseMessageContent = (void*)"Ok";
    _response->m_responseMessageContentSize = 2;
    _response->m_responseStatus = RESPONSE_SEND_MESSAGE;
    std::cout << "Counter: " <<  ++((TestTCPServer*)_applicationInfo)->m_counter << std::endl;
}


int OnError(ServerResult _errorCode, const char* _errorMessage, void* _applicationInfo) {
    std::cout << _errorMessage << std::endl;

    return 1;
}

} // test

} // nm


int main() {
    nm::test::TestTCPServer testStruct;
    nm::cdr::TCPServer server(static_cast<void*>(&testStruct), nm::test::OnMessage, nm::test::OnError, nullptr, nullptr, 5555, 1000, 4096);
    server.Run();

    return 0;
}


#endif // USE_SERVER_LOGGER