#ifndef __NM_CDR_PROVIDER_HPP__
#define __NM_CDR_PROVIDER_HPP__


#include <string>


namespace nm {

namespace cdr {

class Provider {
public:
    Provider(const std::string& a_processorIpAddress, const unsigned int a_processorPortNumber) : m_processorIpAddress(a_processorIpAddress), m_processorPortNumber(a_processorPortNumber) {}

    void SendCdrFiles();

private:
    std::string m_processorIpAddress;
    unsigned int m_processorPortNumber;
};

} // cdr

} // nm


#endif // __NM_CDR_PROVIDER_HPP__