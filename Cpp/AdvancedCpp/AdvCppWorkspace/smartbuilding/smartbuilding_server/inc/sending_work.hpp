#ifndef NM_SENDING_WORK_HPP
#define NM_SENDING_WORK_HPP


#include <string> // std::string
#include <memory> // std::shared_ptr
#include <utility> // std::pair
#include "icallable.hpp"
#include "tcp_socket.hpp"
#include "blocking_bounded_queue.hpp"
#include "blocking_bounded_queue_destruction_policies.hpp"
#include "remote_devices_sockets_manager.hpp"


namespace smartbuilding
{

class SendingWork : public advcpp::ICallable
{
public:
    SendingWork(std::pair<std::string,infra::TCPSocket::BytesBufferProxy> a_handledBuffer, std::shared_ptr<RemoteDevicesSocketsManager> a_devicesSocketsManager)
    : m_handledBuffer(a_handledBuffer)
    , m_devicesSocketsManager(a_devicesSocketsManager)
    {
    }

    virtual void operator()() override
    {
        std::string deviceID = m_handledBuffer.first;
        std::shared_ptr<infra::TCPSocket> deviceSocket = m_devicesSocketsManager->Find(deviceID);
        if(deviceSocket)
        {
            infra::TCPSocket::BytesBufferProxy dataBuffer = m_handledBuffer.second;
            deviceSocket->Send(dataBuffer);
        }
    }

private:
    std::pair<std::string,infra::TCPSocket::BytesBufferProxy> m_handledBuffer;
    std::shared_ptr<RemoteDevicesSocketsManager> m_devicesSocketsManager;
};

} // smartbuilding


#endif // NM_SENDING_WORK_HPP
