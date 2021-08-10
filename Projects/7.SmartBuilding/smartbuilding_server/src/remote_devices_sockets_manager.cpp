#include "remote_devices_sockets_manager.hpp"
#include <string> // std::string
#include <memory> // std::shared_ptr
#include <unordered_map>
#include "tcp_socket.hpp"


void smartbuilding::RemoteDevicesSocketsManager::Insert(const std::string& a_idAsKey, std::shared_ptr<infra::TCPSocket> a_socketAsValue)
{
    m_devicesIDsToSocketsTable.insert({a_idAsKey, a_socketAsValue});
}


void smartbuilding::RemoteDevicesSocketsManager::Remove(const std::string& a_idAsKey)
{
    m_devicesIDsToSocketsTable.erase(a_idAsKey);
}


std::shared_ptr<infra::TCPSocket> smartbuilding::RemoteDevicesSocketsManager::Find(const std::string& a_idAsKey)
{
    if(m_devicesIDsToSocketsTable.find(a_idAsKey) == m_devicesIDsToSocketsTable.end())
    {
        return nullptr;
    }

    return m_devicesIDsToSocketsTable[a_idAsKey];
}
