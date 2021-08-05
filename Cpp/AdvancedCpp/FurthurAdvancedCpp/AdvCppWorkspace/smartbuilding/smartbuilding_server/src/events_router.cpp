#include "events_router.hpp"
#include <stdexcept> // std::runtime_error
#include <memory> // std::shared_ptr
#include "events_subscription_organizer.hpp"
#include "blocking_bounded_queue.hpp"
#include "blocking_bounded_queue_destruction_policies.hpp"
#include "events_dispatcher.hpp"


namespace smartbuilding
{

EventsRouter::EventsRouter(std::shared_ptr<EventsSubscriptionOrganizer> a_subscribersOrganizer)
: m_subscribersOrganizer(a_subscribersOrganizer)
, m_eventsNotifier()
{
}


void EventsRouter::UpdateSubscribersOrganizer(std::shared_ptr<EventsSubscriptionOrganizer> a_newSubscribersOrganizer)
{
    m_subscribersOrganizer = a_newSubscribersOrganizer;
}


void EventsRouter::RouteEvent(Event a_event, std::shared_ptr<advcpp::BlockingBoundedQueue<std::pair<std::string,infra::TCPSocket::BytesBufferProxy>, advcpp::NoOperationPolicy<std::pair<std::string,infra::TCPSocket::BytesBufferProxy>>>> a_handledBuffersQueue)
{
    EventsSubscriptionOrganizer::SubscribersContainer subscribersToAlert;
    bool isValidCollection = m_subscribersOrganizer->FetchRelevantSubscribers(a_event.Type(), a_event.Location(), subscribersToAlert);
    if(!isValidCollection)
    {
        throw std::runtime_error("An unknown internal error has occurred");
    }

    Alert(subscribersToAlert, a_event, a_handledBuffersQueue);
}


void EventsRouter::Alert(EventsSubscriptionOrganizer::SubscribersContainer& a_subscribersToAlert, const Event& a_event, std::shared_ptr<advcpp::BlockingBoundedQueue<std::pair<std::string,infra::TCPSocket::BytesBufferProxy>, advcpp::NoOperationPolicy<std::pair<std::string,infra::TCPSocket::BytesBufferProxy>>>> a_handledBuffersQueue)
{
    m_eventsNotifier.Invoke(a_subscribersToAlert, a_event, a_handledBuffersQueue);
}

} // smartbuilding
