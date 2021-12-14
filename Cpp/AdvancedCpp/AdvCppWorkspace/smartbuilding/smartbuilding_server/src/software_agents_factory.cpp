#include "software_agents_factory.hpp"
#include <string> // std::string
#include <memory> // std::shared_ptr
#include <fstream> // std::ifstream
#include <algorithm> // std::for_each
#include "ini.h"
#include "software_agents_manager.hpp"
#include "so_loader.hpp"
#include "serialized_object.hpp"


smartbuilding::SoftwareAgentsFactory::SoftwareAgentsFactory(std::shared_ptr<SoftwareAgentsManager> a_agentsCollection, std::shared_ptr<SafeLoggersManager> a_loggersManager, std::shared_ptr<IConfigReader> a_configReader)
: m_agentsCollection(a_agentsCollection)
, m_loggersManager(a_loggersManager)
, m_configReader(a_configReader)
{
}


void smartbuilding::SoftwareAgentsFactory::CreateAgents(const std::string& a_configFileName)
{
    // Read config file:
    smartbuilding::IConfigReader::SerializedObjectCollection serializedObjects = m_configReader->ReadConfig(a_configFileName);
    std::for_each(serializedObjects.begin(), serializedObjects.end(),[&](const SerializedObject& a_serializedObject)
    {
        try
        {
            SoLoader dynLib(a_serializedObject.m_soName);
            AgentFactory makeAgent = dynLib.Fetch<AgentFactory>("MakeAgent");
            std::shared_ptr<SoftwareAgent> newAgent = std::shared_ptr<SoftwareAgent>((*makeAgent)(a_serializedObject.m_id, a_serializedObject.m_type, a_serializedObject.m_room, a_serializedObject.m_floor, a_serializedObject.m_configurations, m_loggersManager->GetLogger(a_serializedObject.m_logFileName)));
            m_agentsCollection->Add(newAgent);
        }
        catch(...) // For exception safety
        {
            // Do nothing
        }
    });
}
