#ifndef __NM_CDR_PROVIDER_HPP__
#define __NM_CDR_PROVIDER_HPP__


#include <string>
#include "IFormatter.hpp"


namespace nm {

namespace cdr {

class Provider {
public:
    Provider(const std::string& a_processorIpAddress, const unsigned int a_processorPortNumber, IFormatter* a_fileFormatter, const std::string& a_cdrFilesDirectoryPath, unsigned int a_providingFilesTimeAmountInMinutes) : m_processorIpAddress(a_processorIpAddress), m_processorPortNumber(a_processorPortNumber), m_fileFormatter(a_fileFormatter), m_cdrFilesDirectoryPath(a_cdrFilesDirectoryPath), m_providingFilesTimeAmountInSeconds(a_providingFilesTimeAmountInMinutes * Provider::SECONDS_IN_ONE_MINUTE) {}

    void Run() const;

private:
    unsigned int SleepingAmount() const { return this->m_providingFilesTimeAmountInSeconds; }
    void ProvideFiles() const;

    static const unsigned int THREADS_NUMBER = 4; // TODO: use configuration file
    static const unsigned int WORKING_TASKS_QUEUE_SIZE = 39; // TODO: use configuration file
    static const unsigned int SECONDS_IN_ONE_MINUTE = 60;

    std::string m_processorIpAddress;
    unsigned int m_processorPortNumber;
    IFormatter* m_fileFormatter;
    std::string m_cdrFilesDirectoryPath;
    unsigned int m_providingFilesTimeAmountInSeconds;
};

} // cdr

} // nm


#endif // __NM_CDR_PROVIDER_HPP__