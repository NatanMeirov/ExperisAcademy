#ifndef NM_SOFTWARE_AGENT_HPP
#define NM_SOFTWARE_AGENT_HPP


#include <memory> // std::shared_ptr
#include <string> // std::string
#include "ilogger.hpp"


namespace smartbuilding
{

// An abstract class that represents each remote hardware device of the Smart Building System
class SoftwareAgent
{
public:
    virtual ~SoftwareAgent() = default;

    std::string Configurations() const;
    std::string LogFileName() const;
    std::string RemoteDeviceID() const;

protected:
    // Protected c'tor - to provide the correct using of this class as an abstract base class
    SoftwareAgent(const std::string& a_configurations, std::shared_ptr<ILogger> a_logger, const std::string& a_logFileName, const std::string& a_remoteDeviceID);

    void Log(const std::string& a_message, ILogger::LogLevel a_logLevel);

private:
    std::string m_configurations;
    std::shared_ptr<ILogger> m_logger;
    std::string m_logFileName; // FIXME: check if this attribute should be removed (passing a pointer to the logger pre-initialized with the correct log file stream)
    std::string m_remoteDeviceID;
};

} // smartbuilding


#endif // NM_SOFTWARE_AGENT_HPP
