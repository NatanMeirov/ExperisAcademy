#include "../inc/RoutingStrategy.hpp"
#include <string> // std::string
#include "../inc/RoutingParams.hpp"


std::string nm::cdr::MsisdnRoutingStrategy::GetRoute(const std::string& m_mainRouteOption, RoutingParams* a_routingParams) {
    return std::string("query/") + m_mainRouteOption + "/" + a_routingParams->m_msisdn;
}


std::string nm::cdr::OperatorRoutingStrategy::GetRoute(const std::string& m_mainRouteOption, RoutingParams* a_routingParams) {
    return std::string("query/") + m_mainRouteOption + "/" + a_routingParams->m_mccmnc;
}


std::string nm::cdr::LinkRoutingStrategy::GetRoute(const std::string& m_mainRouteOption, RoutingParams* a_routingParams) {
    return std::string("query/") + m_mainRouteOption + "/" + a_routingParams->m_firstParty + "/" + a_routingParams->m_secondParty;
}