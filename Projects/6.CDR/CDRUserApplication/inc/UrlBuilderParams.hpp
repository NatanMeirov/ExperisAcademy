#ifndef __NM_CDR_URLBUILDERPARAMS_HPP__
#define __NM_CDR_URLBUILDERPARAMS_HPP__


#include <string> // std::string
#include "RequestBuilderParams.hpp"
#include "RoutingParams.hpp"
#include "RoutingStrategy.hpp"


namespace nm {

namespace cdr {

struct UrlBuilderParams : public RequestBuilderParams {
    std::string m_destServerIpAddress;
    unsigned int m_destServerPortNumber;
    bool m_isSecuredHTTP;
    std::string m_mainRouteOption; // To be the base strategy of picking the correct routing option of the url (In our case: "msisdn", "operator", "link")
    RoutingParams* m_routingParams; // additional params
    RoutingStrategy* m_routingStrategy;
};

} // cdr

} // nm


#endif // __NM_CDR_URLBUILDERPARAMS_HPP__