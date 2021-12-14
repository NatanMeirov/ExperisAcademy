#ifndef NM_ROUTING_WORK_HPP
#define NM_ROUTING_WORK_HPP


#include <string> // std::string
#include <memory> // std::shared_ptr
#include <utility> // std::pair
#include "icallable.hpp"
#include "tcp_socket.hpp"
#include "event.hpp"
#include "blocking_bounded_queue.hpp"
#include "blocking_bounded_queue_destruction_policies.hpp"
#include "events_router.hpp"


namespace smartbuilding
{

class RoutingWork : public advcpp::ICallable
{
public:
    RoutingWork(std::shared_ptr<advcpp::BlockingBoundedQueue<std::pair<std::string,infra::TCPSocket::BytesBufferProxy>, advcpp::NoOperationPolicy<std::pair<std::string,infra::TCPSocket::BytesBufferProxy>>>> a_handledBuffersQueueToFill, std::shared_ptr<EventsRouter> a_eventsRouter, const Event& a_eventToRoute)
    : m_handledBuffersQueueToFill(a_handledBuffersQueueToFill)
    , m_eventsRouter(a_eventsRouter)
    , m_eventToRoute(a_eventToRoute)
    {
    }

    virtual void operator()() override
    {
        m_eventsRouter->RouteEvent(m_eventToRoute, m_handledBuffersQueueToFill);
    }

private:
    std::shared_ptr<advcpp::BlockingBoundedQueue<std::pair<std::string,infra::TCPSocket::BytesBufferProxy>, advcpp::NoOperationPolicy<std::pair<std::string,infra::TCPSocket::BytesBufferProxy>>>> m_handledBuffersQueueToFill;
    std::shared_ptr<EventsRouter> m_eventsRouter;
    Event m_eventToRoute;
};

} // smartbuilding


#endif // NM_ROUTING_WORK_HPP
