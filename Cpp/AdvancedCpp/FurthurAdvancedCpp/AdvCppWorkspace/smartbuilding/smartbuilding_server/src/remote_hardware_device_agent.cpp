#include "remote_hardware_device_agent.hpp"
#include <memory> // std::shared_ptr
#include <string> // std::string
#include "ilogger.hpp"


smartbuilding::RemoteHardwareDeviceAgent::RemoteHardwareDeviceAgent(std::shared_ptr<ILogger> a_logger, const std::string& a_logFileName, const std::string& a_remoteDeviceID, const std::string& a_remoteDeviceIP, unsigned int a_remoteDevicePort)
: m_logger(a_logger)
, m_logFileName(a_logFileName)
, m_remoteDeviceID(a_remoteDeviceID)
, m_remoteDeviceIP(a_remoteDeviceIP)
, m_remoteDevicePort(a_remoteDevicePort)
{
}


std::string smartbuilding::RemoteHardwareDeviceAgent::LogFileName() const
{
    return m_logFileName;
}


std::string smartbuilding::RemoteHardwareDeviceAgent::RemoteDeviceID() const
{
    return m_remoteDeviceID;
}


std::string smartbuilding::RemoteHardwareDeviceAgent::RemoteDeviceIP() const
{
    return m_remoteDeviceIP;
}


unsigned int smartbuilding::RemoteHardwareDeviceAgent::RemoteDevicePort() const
{
    return m_remoteDevicePort;
}


void smartbuilding::RemoteHardwareDeviceAgent::Log(const std::string& a_message, ILogger::LogLevel a_logLevel)
{
    m_logger->Log(a_message, a_logLevel);
}