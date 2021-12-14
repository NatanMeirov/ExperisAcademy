#ifndef NM_REMOTE_DEVICES_SOCKETS_MANAGER_HPP
#define NM_REMOTE_DEVICES_SOCKETS_MANAGER_HPP


#include <string> // std::string
#include <memory> // std::shared_ptr
#include <unordered_map>
#include "tcp_socket.hpp"


namespace smartbuilding
{

class RemoteDevicesSocketsManager
{
public:
    RemoteDevicesSocketsManager() = default;
    RemoteDevicesSocketsManager(const RemoteDevicesSocketsManager& a_other) = default;
    RemoteDevicesSocketsManager& operator=(const RemoteDevicesSocketsManager& a_other) = default;
    ~RemoteDevicesSocketsManager() = default;

    void Insert(const std::string& a_idAsKey, std::shared_ptr<infra::TCPSocket> a_socketAsValue);
    void Remove(const std::string& a_idAsKey);
    std::shared_ptr<infra::TCPSocket> Find(const std::string& a_idAsKey); // Returns nullptr if ID has not found

private:
    std::unordered_map<std::string, std::shared_ptr<infra::TCPSocket>> m_devicesIDsToSocketsTable;
};

} // smartbuilding


#endif // NM_REMOTE_DEVICES_SOCKETS_MANAGER_HPP
