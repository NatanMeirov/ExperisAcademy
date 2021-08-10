#ifndef NM_SOFTWARE_AGENTS_FACTORY_HPP
#define NM_SOFTWARE_AGENTS_FACTORY_HPP


#include <string> // std::string
#include <memory> // std::shared_ptr
#include "software_agent.hpp"
#include "software_agents_manager.hpp"
#include "safe_loggers_manager.hpp"
#include "iconfig_reader.hpp"


namespace smartbuilding
{

class SoftwareAgentsFactory
{
public:
    SoftwareAgentsFactory(std::shared_ptr<SoftwareAgentsManager> a_agentsCollection, std::shared_ptr<SafeLoggersManager> a_loggersManager, std::shared_ptr<IConfigReader> a_configReader);
    SoftwareAgentsFactory(const SoftwareAgentsFactory& a_other) = default;
    SoftwareAgentsFactory& operator=(const SoftwareAgentsFactory& a_other) = default;
    ~SoftwareAgentsFactory() = default;

    void CreateAgents(const std::string& a_configFileName);

private:
    typedef SoftwareAgent* (*AgentFactory)(const std::string& a_deviceID, const std::string& a_deviceType, unsigned int a_room, unsigned int a_floor, const std::string& a_configurations, std::shared_ptr<ILogger> a_logger);

private:
    std::shared_ptr<SoftwareAgentsManager> m_agentsCollection;
    std::shared_ptr<SafeLoggersManager> m_loggersManager;
    std::shared_ptr<IConfigReader> m_configReader;
};

} // smartbuilding


#endif // NM_SOFTWARE_AGENTS_FACTORY_HPP
