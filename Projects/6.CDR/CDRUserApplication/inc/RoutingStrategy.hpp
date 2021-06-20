#ifndef __NM_CDR_ROUTING_STRATEGY_HPP__
#define __NM_CDR_ROUTING_STRATEGY_HPP__


#include <string> // std::string
#include "RoutingParams.hpp"


namespace nm {

namespace cdr {

struct RoutingStrategy {
    virtual std::string GetRoute(const std::string& m_mainRouteOption, RoutingParams* a_routingParams) = 0;
};


struct MsisdnRoutingStrategy : public RoutingStrategy {
    virtual std::string GetRoute(const std::string& m_mainRouteOption, RoutingParams* a_routingParams) override;
};


struct OperatorRoutingStrategy : public RoutingStrategy {
    virtual std::string GetRoute(const std::string& m_mainRouteOption, RoutingParams* a_routingParams) override;
};


struct LinkRoutingStrategy : public RoutingStrategy {
    virtual std::string GetRoute(const std::string& m_mainRouteOption, RoutingParams* a_routingParams) override;
};

} // cdr

} // nm


#endif // __NM_CDR_ROUTING_STRATEGY_HPP__