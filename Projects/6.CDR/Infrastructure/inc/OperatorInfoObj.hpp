#ifndef __NM_CDR_OPERATORINFOOBJ_HPP__
#define __NM_CDR_OPERATORINFOOBJ_HPP__


#include <cstddef> // size_t
#include "InfoObj.hpp"


namespace nm {

namespace cdr {

struct OperatorInfoObj : public InfoObj {
    OperatorInfoObj() : m_totalIncomingVoiceCallDuration(0), m_totalOutgoingVoiceCallDuration(0), m_totalIncomingSms(0), m_totalOutgoingSms(0) {}

    size_t m_totalIncomingVoiceCallDuration;
    size_t m_totalOutgoingVoiceCallDuration;
    size_t m_totalIncomingSms;
    size_t m_totalOutgoingSms;
};

} // cdr

} // nm


#endif // __NM_CDR_OPERATORINFOOBJ_HPP__