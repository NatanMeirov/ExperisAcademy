#include "published_events_transmit_work.hpp"
#include <string> // std::string
#include <memory> // std::shared_ptr, std::make_shared
#include <utility> // std::pair
#include "icallable.hpp"
#include "tcp_socket.hpp"
#include "event.hpp"
#include "blocking_bounded_queue.hpp"
#include "blocking_bounded_queue_destruction_policies.hpp"
#include "thread_pool.hpp"
#include "thread_pool_destruction_policies.hpp"
#include "events_router.hpp"
#include "routing_work.hpp"


smartbuilding::PublishedEventsTransmitWork::PublishedEventsTransmitWork(std::shared_ptr<advcpp::BlockingBoundedQueue<Event, advcpp::NoOperationPolicy<Event>>> a_publishedEventsQueueToDequeueFrom, std::shared_ptr<advcpp::BlockingBoundedQueue<std::pair<std::string,infra::TCPSocket::BytesBufferProxy>, advcpp::NoOperationPolicy<std::pair<std::string,infra::TCPSocket::BytesBufferProxy>>>> a_handledBuffersQueueToFill, std::shared_ptr<advcpp::ThreadPool<advcpp::ShutdownPolicy<>>> a_routingWorkers, std::shared_ptr<EventsRouter> a_eventsRouter)
: m_publishedEventsQueueToDequeueFrom(a_publishedEventsQueueToDequeueFrom)
, m_handledBuffersQueueToFill(a_handledBuffersQueueToFill)
, m_routingWorkers(a_routingWorkers)
, m_eventsRouter(a_eventsRouter)
{
}


void smartbuilding::PublishedEventsTransmitWork::operator()()
{
    while(true)
    {
        Event newPublishedEvent;
        m_publishedEventsQueueToDequeueFrom->Dequeue(newPublishedEvent);

        try
        {
            std::shared_ptr<RoutingWork> routingWork = std::make_shared<RoutingWork>(m_handledBuffersQueueToFill, m_eventsRouter, newPublishedEvent);
            m_routingWorkers->SubmitWork(routingWork);
        }
        catch(...)
        {
            // For exception safety
        }
    }
}
