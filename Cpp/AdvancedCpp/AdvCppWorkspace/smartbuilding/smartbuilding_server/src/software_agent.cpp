#include "software_agent.hpp"
#include <memory> // std::shared_ptr
#include <string> // std::string
#include "ilogger.hpp"


smartbuilding::SoftwareAgent::SoftwareAgent(const std::string& a_configurations, std::shared_ptr<ILogger> a_logger, const std::string& a_remoteDeviceID, const Location& a_location)
: m_configurations(a_configurations)
, m_logger(a_logger)
, m_remoteDeviceID(a_remoteDeviceID)
, m_location(a_location)
{
}


std::string smartbuilding::SoftwareAgent::Configurations() const
{
    return m_configurations;
}


std::string smartbuilding::SoftwareAgent::RemoteDeviceID() const
{
    return m_remoteDeviceID;
}


smartbuilding::Location smartbuilding::SoftwareAgent::Loc() const
{
    return m_location;
}


void smartbuilding::SoftwareAgent::Log(const std::string& a_message, ILogger::LogLevel a_logLevel)
{
    m_logger->Log(a_message, a_logLevel);
}
