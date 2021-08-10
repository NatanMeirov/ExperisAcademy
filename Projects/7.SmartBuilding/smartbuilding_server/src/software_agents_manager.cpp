#include "software_agents_manager.hpp"
#include <string> // std::string
#include <memory> // std::shared_ptr
#include <unordered_map>
#include "software_agent.hpp"


namespace smartbuilding
{

void SoftwareAgentsManager::Add(std::shared_ptr<SoftwareAgent> a_agent)
{
    m_agentsTable[a_agent->RemoteDeviceID()] = a_agent;
}


void SoftwareAgentsManager::RemoveByID(const std::string& a_id)
{
    m_agentsTable.erase(a_id);
}


std::shared_ptr<SoftwareAgent> SoftwareAgentsManager::FindByID(const std::string& a_id)
{
    if(m_agentsTable.find(a_id) == m_agentsTable.end()) // Key has not found
    {
        return nullptr;
    }

    return m_agentsTable[a_id];
}

} // smartbuilding
