#ifndef NM_PUBLISHED_EVENTS_TRANSMIT_WORK_HPP
#define NM_PUBLISHED_EVENTS_TRANSMIT_WORK_HPP


#include <string> // std::string
#include <memory> // std::shared_ptr
#include <utility> // std::pair
#include "icallable.hpp"
#include "tcp_socket.hpp"
#include "event.hpp"
#include "blocking_bounded_queue.hpp"
#include "blocking_bounded_queue_destruction_policies.hpp"
#include "thread_pool.hpp"
#include "thread_pool_destruction_policies.hpp"
#include "events_router.hpp"


namespace smartbuilding
{

class PublishedEventsTransmitWork : public advcpp::ICallable
{
public:
    PublishedEventsTransmitWork(std::shared_ptr<advcpp::BlockingBoundedQueue<Event, advcpp::NoOperationPolicy<Event>>> a_publishedEventsQueueToDequeueFrom, std::shared_ptr<advcpp::BlockingBoundedQueue<std::pair<std::string,infra::TCPSocket::BytesBufferProxy>, advcpp::NoOperationPolicy<std::pair<std::string,infra::TCPSocket::BytesBufferProxy>>>> a_handledBuffersQueueToFill, std::shared_ptr<advcpp::ThreadPool<advcpp::ShutdownPolicy<>>> a_routingWorkers, std::shared_ptr<EventsRouter> a_eventsRouter);

    virtual void operator()() override;

private:
    std::shared_ptr<advcpp::BlockingBoundedQueue<Event, advcpp::NoOperationPolicy<Event>>> m_publishedEventsQueueToDequeueFrom;
    std::shared_ptr<advcpp::BlockingBoundedQueue<std::pair<std::string,infra::TCPSocket::BytesBufferProxy>, advcpp::NoOperationPolicy<std::pair<std::string,infra::TCPSocket::BytesBufferProxy>>>> m_handledBuffersQueueToFill;
    std::shared_ptr<advcpp::ThreadPool<advcpp::ShutdownPolicy<>>> m_routingWorkers;
    std::shared_ptr<EventsRouter> m_eventsRouter;
};

} // smartbuilding


#endif // NM_PUBLISHED_EVENTS_TRANSMIT_WORK_HPP
