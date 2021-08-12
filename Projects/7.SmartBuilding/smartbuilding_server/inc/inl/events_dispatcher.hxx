#ifndef NM_EVENTS_DISPATCHER_HXX
#define NM_EVENTS_DISPATCHER_HXX


#include <algorithm> // std::for_each
#include <type_traits> // std::is_same
#include <memory> // std::shared_ptr
#include "blocking_bounded_queue.hpp"
#include "blocking_bounded_queue_destruction_policies.hpp"
#include "isubscriber.hpp"
#include "invoker_work.hpp"


namespace smartbuilding
{

inline EventsDispatcher::EventsDispatcher()
: m_invokers(advcpp::ShutdownPolicy<>(), WORKERS_QUEUE_SIZE)
{
}


inline EventsDispatcher::~EventsDispatcher()
{
    m_invokers.Shutdown();
}


template <typename C>
void EventsDispatcher::Invoke(C a_subscribersCollection, const Event& a_event, std::shared_ptr<advcpp::BlockingBoundedQueue<std::pair<std::string,infra::TCPSocket::BytesBufferProxy>, advcpp::NoOperationPolicy<std::pair<std::string,infra::TCPSocket::BytesBufferProxy>>>> a_handledBuffersQueue) noexcept
{
    static_assert(std::is_same<typename C::value_type, std::shared_ptr<ISubscriber>>::value, "C::value_type must be of type ISubscriber*");

    std::for_each(a_subscribersCollection.begin(), a_subscribersCollection.end(), [&](std::shared_ptr<ISubscriber> a_subscriber)
    {
        try
        {
            std::shared_ptr<InvokerWork> invokeTask(new InvokerWork(a_subscriber, a_event, a_handledBuffersQueue));
            m_invokers.SubmitWork(invokeTask);
        }
        catch(...)
        {
            // For exception safety
        }
    });
}

} // smartbuilding


#endif // NM_EVENTS_DISPATCHER_HXX
