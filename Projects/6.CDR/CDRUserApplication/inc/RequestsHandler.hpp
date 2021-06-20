#ifndef __NM_CDR_REQUESTSHANDLER_HPP__
#define __NM_CDR_REQUESTSHANDLER_HPP__


#include <string> // std::string
#include "../../Infrastructure/Network/TCPSocket.hpp"
#include "../../Infrastructure/JsonSerializer/json.hpp"


namespace nm {

namespace cdr {

// An adapter class to connect between the UIHandler and the REST Api Server
class RequestsHandler {
public:
    RequestsHandler(const std::string& a_ipAddressOfServer, const unsigned int a_portNumberOfServer);
    RequestsHandler(const RequestsHandler& a_other) = delete; // MUST be deleted because of the implementation of the inner socket
    RequestsHandler& operator=(const RequestsHandler& a_other) = delete; // MUST be deleted because of the implementation of the inner socket
    ~RequestsHandler() = default;

    void Request(const std::string& a_request);
    nlohmann::json Response();

private:
    static const unsigned int RESPONSE_MAX_BUFFER_SIZE = 4096; // 4 KB

    nm::TCPSocket m_connectionSocket;
};

} // cdr

} // nm


#endif // __NM_CDR_REQUESTSHANDLER_HPP__