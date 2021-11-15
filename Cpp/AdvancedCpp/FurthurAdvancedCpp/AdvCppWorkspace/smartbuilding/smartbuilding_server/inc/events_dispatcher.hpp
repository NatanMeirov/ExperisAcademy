#ifndef NM_EVENTS_DISPATCHER_HPP
#define NM_EVENTS_DISPATCHER_HPP


#include <memory> // std::shared_ptr
#include "isubscriber.hpp"
#include "event.hpp"
#include "blocking_bounded_queue.hpp"
#include "blocking_bounded_queue_destruction_policies.hpp"
#include "thread_pool.hpp"
#include "thread_pool_destruction_policies.hpp"


namespace smartbuilding
{

class EventsDispatcher
{
public:
    EventsDispatcher();
    EventsDispatcher(const EventsDispatcher& a_other) = delete;
    EventsDispatcher& operator=(const EventsDispatcher& a_other) = delete;
    ~EventsDispatcher();

    // Concept of C: C must be an iterable container (implement begin() and end()), must have value_type info (typedef), and C::value_type must be ISubscriber*
    template <typename C>
    void Invoke(C a_subscribersCollection, const Event& a_event, std::shared_ptr<advcpp::BlockingBoundedQueue<std::pair<std::string,infra::TCPSocket::BytesBufferProxy>, advcpp::NoOperationPolicy<std::pair<std::string,infra::TCPSocket::BytesBufferProxy>>>> a_handledBuffersQueue) noexcept;

private:
    static const unsigned int WORKS_QUEUE_SIZE = 100; // TODO: in version 2, read this constant from a configuration file

private:
    advcpp::ThreadPool<advcpp::ShutdownPolicy<>> m_invokers;
};

} // smartbuilding


#include "inl/events_dispatcher.hxx"


#endif // NM_EVENTS_DISPATCHER_HPP
