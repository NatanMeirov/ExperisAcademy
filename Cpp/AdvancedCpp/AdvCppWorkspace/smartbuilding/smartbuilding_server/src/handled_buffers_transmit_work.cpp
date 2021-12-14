#include "handled_buffers_transmit_work.hpp"
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
#include "sending_work.hpp"


smartbuilding::HandledBuffersTransmitWork::HandledBuffersTransmitWork(std::shared_ptr<advcpp::BlockingBoundedQueue<std::pair<std::string,infra::TCPSocket::BytesBufferProxy>, advcpp::NoOperationPolicy<std::pair<std::string,infra::TCPSocket::BytesBufferProxy>>>> a_handledBuffersQueue, std::shared_ptr<advcpp::ThreadPool<advcpp::ShutdownPolicy<>>> a_sendingWorkers, std::shared_ptr<RemoteDevicesSocketsManager> a_devicesSocketsManager)
: m_handledBuffersQueue(a_handledBuffersQueue)
, m_sendingWorkers(a_sendingWorkers)
, m_devicesSocketsManager(a_devicesSocketsManager)
{
}


void smartbuilding::HandledBuffersTransmitWork::operator()()
{
    while(true)
    {
        std::pair<std::string,infra::TCPSocket::BytesBufferProxy> handledBuffer;
        m_handledBuffersQueue->Dequeue(handledBuffer);

        try
        {
            std::shared_ptr<SendingWork> sendingWork = std::make_shared<SendingWork>(handledBuffer, m_devicesSocketsManager);
            m_sendingWorkers->SubmitWork(sendingWork);
        }
        catch(...)
        {
            // For exception safety
        }
    }
}
