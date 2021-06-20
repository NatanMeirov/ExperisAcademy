#include "../inc/UrlBuilder.hpp"
#include <string> // std::string, std::to_string
#include "../inc/RoutingParams.hpp"
#include "../inc/RoutingStrategy.hpp"


nm::cdr::UrlBuilder::UrlBuilder(UrlBuilderParams* a_params)
: m_params(a_params) {
}


std::string nm::cdr::UrlBuilder::BuildRequest() {
    return std::string("http") + (this->m_params->m_isSecuredHTTP ? "s" : "") + "://"
    + this->m_params->m_destServerIpAddress + ":"
    + std::to_string(this->m_params->m_destServerPortNumber) + "/"
    + this->m_params->m_routingStrategy->GetRoute(this->m_params->m_mainRouteOption, this->m_params->m_routingParams);
}