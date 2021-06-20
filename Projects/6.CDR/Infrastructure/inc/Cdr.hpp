#ifndef __NM_CDR_CDR_HPP__
#define __NM_CDR_CDR_HPP__


#include <cstdint>
#include <string> // std::string


namespace nm {

namespace cdr {

struct Cdr {
    enum UsageType { MOC, MTC, SMS_MO, SMS_MT, D, U, B, X };

    uint64_t m_sequenceNumber;
    uint64_t m_imsi;
    std::string m_imei;
    UsageType m_type;
    std::string m_msisdn;
    std::string m_callDate; // DD/MM/YYYY
    std::string m_callTime; // HH:MM:SS
    unsigned int m_duration; // In seconds
    uint64_t m_bytesReceived; // If type is D (data)
    uint64_t m_bytesTransmitted; // If type is D (data)
    uint64_t m_imsiOfSecondParty;
    std::string m_msisdnOfSecondParty;
};

} // cdr

} // nm


#endif // __NM_CDR_CDR_HPP__