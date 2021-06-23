#ifndef __NM_CDR_SECONDPARTYINFO_HPP__
#define __NM_CDR_SECONDPARTYINFO_HPP__


#include <cstddef> // size_t


namespace nm {

namespace cdr {

struct SecondPartyInfo {
    size_t m_totalVoiceCallDuration;
    size_t m_totalSmsExchanged;
};

} // cdr

} // nm


#endif // __NM_CDR_SECONDPARTYINFO_HPP__