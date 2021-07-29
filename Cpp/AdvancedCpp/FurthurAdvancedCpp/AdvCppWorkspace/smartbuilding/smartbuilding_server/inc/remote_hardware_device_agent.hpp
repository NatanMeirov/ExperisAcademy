#ifndef NM_REMOTE_HARDWARE_DEVICE_AGENT_HPP
#define NM_REMOTE_HARDWARE_DEVICE_AGENT_HPP


#include <memory> // std::shared_ptr
#include <string> // std::string
#include "ilogger.hpp"


namespace smartbuilding
{

// An abstract class that represents each remote hardware device of the Smart Building System
class RemoteHardwareDeviceAgent
{
public:
    virtual ~RemoteHardwareDeviceAgent() = default;

    std::string LogFileName() const;
    std::string RemoteDeviceID() const;
    std::string RemoteDeviceIP() const;
    unsigned int RemoteDevicePort() const;

protected:
    // Protected c'tor - to provide the correct using of this class as an abstract base class
    RemoteHardwareDeviceAgent(std::shared_ptr<ILogger> a_logger, const std::string& a_logFileName, const std::string& a_remoteDeviceID, const std::string& a_remoteDeviceIP, unsigned int a_remoteDevicePort);

    void Log(const std::string& a_message, ILogger::LogLevel a_logLevel);

private:
    std::shared_ptr<ILogger> m_logger;
    std::string m_logFileName; // FIXME: check if this attribute should be removed (passing a pointer to the logger pre-initialized with the correct log file stream)
    std::string m_remoteDeviceID;
    std::string m_remoteDeviceIP;
    unsigned int m_remoteDevicePort;
};

} // smartbuilding


#endif // NM_REMOTE_HARDWARE_DEVICE_AGENT_HPP