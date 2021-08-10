#ifndef NM_EVENTS_ROUTER_HPP
#define NM_EVENTS_ROUTER_HPP


#include <unordered_map> // std::unordered_map
#include <memory> // std::shared_ptr
#include "isubscriber.hpp"
#include "event.hpp"
#include "events_subscription_organizer.hpp"
#include "blocking_bounded_queue.hpp"
#include "blocking_bounded_queue_destruction_policies.hpp"
#include "events_dispatcher.hpp"


namespace smartbuilding
{

class EventsRouter
{
public:
    explicit EventsRouter(std::shared_ptr<EventsSubscriptionOrganizer> a_subscribersOrganizer);
    EventsRouter(const EventsRouter& a_other) = delete;
    EventsRouter& operator=(const EventsRouter& a_other) = delete;
    ~EventsRouter() = default;

    void RouteEvent(Event a_event, std::shared_ptr<advcpp::BlockingBoundedQueue<std::pair<std::string,infra::TCPSocket::BytesBufferProxy>, advcpp::NoOperationPolicy<std::pair<std::string,infra::TCPSocket::BytesBufferProxy>>>> a_handledBuffersQueue); // Passes the event by copy (cannot ensure that the reference to the event is still valid)

private:
    void Alert(EventsSubscriptionOrganizer::SubscribersContainer& a_subscribersToAlert, const Event& a_event, std::shared_ptr<advcpp::BlockingBoundedQueue<std::pair<std::string,infra::TCPSocket::BytesBufferProxy>, advcpp::NoOperationPolicy<std::pair<std::string,infra::TCPSocket::BytesBufferProxy>>>> a_handledBuffersQueue);

private:
    EventsDispatcher m_eventsNotifier;
    std::shared_ptr<EventsSubscriptionOrganizer> m_subscribersOrganizer; // The subscribers Database
};

} // smartbuilding


#endif // NM_EVENTS_ROUTER_HPP
