#ifndef NM_SOFTWARE_AGENTS_MANAGER_HPP
#define NM_SOFTWARE_AGENTS_MANAGER_HPP


#include <string> // std::string
#include <memory> // std::shared_ptr
#include <unordered_map>
#include "software_agent.hpp"


namespace smartbuilding
{

class SoftwareAgentsManager
{
public:
    SoftwareAgentsManager() = default;
    SoftwareAgentsManager(const SoftwareAgentsManager& a_other) = delete;
    SoftwareAgentsManager& operator=(const SoftwareAgentsManager& a_other) = delete;
    ~SoftwareAgentsManager() = default;

    void Add(std::shared_ptr<SoftwareAgent> a_agent);
    void RemoveByID(const std::string& a_id);
    std::shared_ptr<SoftwareAgent> FindByID(const std::string& a_id); // Returns nullptr if ID has not found

private:
    std::unordered_map<std::string, std::shared_ptr<SoftwareAgent>> m_agentsTable; // An ID to SoftwareAgent* map
};

} // smartbuilding


#endif // NM_SOFTWARE_AGENTS_MANAGER_HPP
