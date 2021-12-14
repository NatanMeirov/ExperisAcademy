#ifndef NM_HANDLED_BUFFERS_TRANSMIT_WORK_HPP
#define NM_HANDLED_BUFFERS_TRANSMIT_WORK_HPP


#include <string> // std::string
#include <memory> // std::shared_ptr
#include <utility> // std::pair
#include "icallable.hpp"
#include "tcp_socket.hpp"
#include "blocking_bounded_queue.hpp"
#include "blocking_bounded_queue_destruction_policies.hpp"
#include "thread_pool.hpp"
#include "thread_pool_destruction_policies.hpp"
#include "remote_devices_sockets_manager.hpp"


namespace smartbuilding
{

class HandledBuffersTransmitWork : public advcpp::ICallable
{
public:
    HandledBuffersTransmitWork(std::shared_ptr<advcpp::BlockingBoundedQueue<std::pair<std::string,infra::TCPSocket::BytesBufferProxy>, advcpp::NoOperationPolicy<std::pair<std::string,infra::TCPSocket::BytesBufferProxy>>>> a_handledBuffersQueue, std::shared_ptr<advcpp::ThreadPool<advcpp::ShutdownPolicy<>>> a_sendingWorkers, std::shared_ptr<RemoteDevicesSocketsManager> a_devicesSocketsManager);

    virtual void operator()() override;

private:
    std::shared_ptr<advcpp::BlockingBoundedQueue<std::pair<std::string,infra::TCPSocket::BytesBufferProxy>, advcpp::NoOperationPolicy<std::pair<std::string,infra::TCPSocket::BytesBufferProxy>>>> m_handledBuffersQueue;
    std::shared_ptr<advcpp::ThreadPool<advcpp::ShutdownPolicy<>>> m_sendingWorkers;
    std::shared_ptr<RemoteDevicesSocketsManager> m_devicesSocketsManager;
};

} // smartbuilding


#endif // NM_HANDLED_BUFFERS_TRANSMIT_WORK_HPP
