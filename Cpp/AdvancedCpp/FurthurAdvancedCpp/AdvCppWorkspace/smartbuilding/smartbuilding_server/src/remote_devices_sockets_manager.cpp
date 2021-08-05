#include "remote_devices_sockets_manager.hpp"
#include <string> // std::string
#include <unordered_map>


void smartbuilding::RemoteDevicesSocketsManager::Insert(const std::string& a_idAsKey, TCPSocketID a_socketIdAsValue)
{
    m_devicesIDsToSocketsTable.insert({a_idAsKey, a_socketIdAsValue});
}


void smartbuilding::RemoteDevicesSocketsManager::Remove(const std::string& a_idAsKey)
{
    m_devicesIDsToSocketsTable.erase(a_idAsKey);
}


std::pair<bool, smartbuilding::RemoteDevicesSocketsManager::TCPSocketID> smartbuilding::RemoteDevicesSocketsManager::Find(const std::string& a_idAsKey)
{
    if(m_devicesIDsToSocketsTable.find(a_idAsKey) == m_devicesIDsToSocketsTable.end())
    {
        return std::make_pair(false, -1);
    }

    return std::make_pair(true, m_devicesIDsToSocketsTable[a_idAsKey]);
}
