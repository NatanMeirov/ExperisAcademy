#ifndef NM_REMOTE_DEVICES_SOCKETS_MANAGER_HPP
#define NM_REMOTE_DEVICES_SOCKETS_MANAGER_HPP


#include <string> // std::string
#include <unordered_map>


namespace smartbuilding
{

class RemoteDevicesSocketsManager
{
public:
    using TCPSocketID = int;

    RemoteDevicesSocketsManager() = default;
    RemoteDevicesSocketsManager(const RemoteDevicesSocketsManager& a_other) = default;
    RemoteDevicesSocketsManager& operator=(const RemoteDevicesSocketsManager& a_other) = default;
    ~RemoteDevicesSocketsManager() = default;

    void Insert(const std::string& a_idAsKey, TCPSocketID a_socketIdAsValue);
    void Remove(const std::string& a_idAsKey);
    std::pair<bool, TCPSocketID> Find(const std::string& a_idAsKey); // Returns false (as first) if ID has not found (the socketID value would be the invalid FD value -1)

private:
    std::unordered_map<std::string, TCPSocketID> m_devicesIDsToSocketsTable;
};

} // smartbuilding


#endif // NM_REMOTE_DEVICES_SOCKETS_MANAGER_HPP
