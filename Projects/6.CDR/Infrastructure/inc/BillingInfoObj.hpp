#ifndef __NM_CDR_BILLINGINFOOBJ_HPP__
#define __NM_CDR_BILLINGINFOOBJ_HPP__


#include <cstddef> // size_t
#include <string>
#include <unordered_map>
#include "InfoObj.hpp"
#include "SecondPartyInfo.hpp"


namespace nm {

namespace cdr {

struct BillingInfoObj : public InfoObj {
    size_t m_outgoingVoiceCallDuration;
    size_t m_incomingVoiceCallDuration;
    size_t m_totalDataTransferred;
    size_t m_totalDataReceived;
    size_t m_totalSmsSent;
    size_t m_totalSmsReceived;
    std::unordered_map<std::string, SecondPartyInfo> m_secondPartiesInfoTable; // Key: MSISDN
};

} // cdr

} // nm


#endif // __NM_CDR_BILLINGINFOOBJ_HPP__