#ifndef __NM_CDR_LINKGRAPHINFOOBJ_HPP__
#define __NM_CDR_LINKGRAPHINFOOBJ_HPP__


#include <unordered_map>
#include "InfoObj.hpp"
#include "SecondPartyInfo.hpp"


namespace nm {

namespace cdr {

struct LinkGraphInfoObj : public InfoObj {
    struct SecondParty {
        std::string m_secondPartyMsisdn;
        SecondPartyInfo m_secondPartyInfo;
    };

    LinkGraphInfoObj() : m_graph() {}

    std::unordered_map<uint64_t, SecondParty> m_graph;
};

} // cdr

} // nm


#endif // __NM_CDR_LINKGRAPHINFOOBJ_HPP__