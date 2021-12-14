#ifndef NM_SOFTWARE_AGENT_HPP
#define NM_SOFTWARE_AGENT_HPP


#include <memory> // std::shared_ptr
#include <string> // std::string
#include "ilogger.hpp"
#include "location.hpp"


namespace smartbuilding
{

// An abstract class that represents each remote hardware device of the Smart Building System
class SoftwareAgent
{
public:
    virtual ~SoftwareAgent() = default;

    std::string Configurations() const;
    std::string RemoteDeviceID() const;
    Location Loc() const;

protected:
    // Protected c'tor - to provide the correct using of this class as an abstract base class
    SoftwareAgent(const std::string& a_configurations, std::shared_ptr<ILogger> a_logger, const std::string& a_remoteDeviceID, const Location& a_location);

    void Log(const std::string& a_message, ILogger::LogLevel a_logLevel);

private:
    std::string m_configurations;
    std::shared_ptr<ILogger> m_logger;
    std::string m_remoteDeviceID;
    Location m_location;
};

} // smartbuilding


#endif // NM_SOFTWARE_AGENT_HPP
