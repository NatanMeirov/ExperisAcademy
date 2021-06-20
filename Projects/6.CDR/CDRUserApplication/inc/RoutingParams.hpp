#ifndef __NM_CDR_ROUTINGPARAMS_HPP__
#define __NM_CDR_ROUTINGPARAMS_HPP__


#include <string> // std::string


namespace nm {

namespace cdr {

struct RoutingParams {
    std::string m_msisdn; // msisdn
    std::string m_mccmnc; // operator
    std::string m_firstParty; // link
    std::string m_secondParty; // link
    // could use the m_msisdn instead of the m_firstParty, but added this member for additional readability
};

} // cdr

} // nm


#endif // __NM_CDR_ROUTINGPARAMS_HPP__