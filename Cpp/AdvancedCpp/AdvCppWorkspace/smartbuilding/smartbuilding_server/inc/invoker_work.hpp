#ifndef NM_INVOKER_WORK_HPP
#define NM_INVOKER_WORK_HPP


#include <memory> // std::shared_ptr
#include "icallable.hpp"
#include "blocking_bounded_queue.hpp"
#include "blocking_bounded_queue_destruction_policies.hpp"
#include "isubscriber.hpp"
#include "event.hpp"


namespace smartbuilding
{

class InvokerWork : public advcpp::ICallable
{
public:
    InvokerWork(std::shared_ptr<ISubscriber> a_toInvoke, const Event& a_event, std::shared_ptr<advcpp::BlockingBoundedQueue<std::pair<std::string,infra::TCPSocket::BytesBufferProxy>, advcpp::NoOperationPolicy<std::pair<std::string,infra::TCPSocket::BytesBufferProxy>>>> a_handledBuffersQueue) : m_subscriber(a_toInvoke), m_event(a_event), m_handledBuffersQueue(a_handledBuffersQueue) {}

    virtual void operator()() override
    {
        m_subscriber->Notify(m_event, m_handledBuffersQueue);
    }

private:
    std::shared_ptr<ISubscriber> m_subscriber;
    Event m_event;
    std::shared_ptr<advcpp::BlockingBoundedQueue<std::pair<std::string,infra::TCPSocket::BytesBufferProxy>, advcpp::NoOperationPolicy<std::pair<std::string,infra::TCPSocket::BytesBufferProxy>>>> m_handledBuffersQueue;
};

} // smartbuilding


#endif // NM_INVOKER_WORK_HPP
