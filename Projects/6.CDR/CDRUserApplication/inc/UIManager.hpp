#ifndef __NM_CDR_UIMANAGER_HPP__
#define __NM_CDR_UIMANAGER_HPP__


#include "RequestsHandler.hpp"


namespace nm {

namespace cdr {

class UIManager {
public:
    UIManager(const std::string& a_restApiServerIp, const unsigned int a_restApiServerPort);


private:
    RequestsHandler m_requestsHandler;
    // UrlBuilder m_urlBuilder; TODO - add this builder
    std::string m_restApiServerIp;
    unsigned int m_restApiServerPort;

};

} // cdr

} // nm


#endif // __NM_CDR_UIMANAGER_HPP__