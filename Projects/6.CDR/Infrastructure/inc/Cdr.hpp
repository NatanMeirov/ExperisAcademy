#ifndef __NM_CDR_CDR_HPP__
#define __NM_CDR_CDR_HPP__


#include <cstddef> // size_t
#include <string> // std::string
#include <ctime> // struct tm


namespace nm {

namespace cdr {

struct Cdr {
    enum UsageType { MOC, MTC, SMS_MO, SMS_MT, D, U, B, X };

    size_t m_imsi;
    std::string m_imei;
    UsageType m_type;
    std::string m_msisdn;
    std::string m_callDate; // DD/MM/YYYY
    std::string m_callTime; // HH:MM:SS
    unsigned int m_duration; // In seconds
    size_t m_bytesReceived; // If type is D (data)
    size_t m_bytesTransmitted; // If type is D (data)
    std::string m_msisdnOfSecondParty;
};

} // cdr

} // nm


#endif // __NM_CDR_CDR_HPP__