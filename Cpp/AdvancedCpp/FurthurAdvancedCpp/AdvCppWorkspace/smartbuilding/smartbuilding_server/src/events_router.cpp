#include "events_router.hpp"
#include <stdexcept> // std::runtime_error
#include <memory> // std::shared_ptr
#include "events_subscription_organizer.hpp"
#include "events_dispatcher.hpp"


namespace smartbuilding
{

EventsRouter::EventsRouter(std::shared_ptr<EventsSubscriptionOrganizer> a_subscribersOrganizer)
: m_subscribersOrganizer(a_subscribersOrganizer)
, m_eventsNotifier()
{
}


void EventsRouter::RouteEvent(Event a_event)
{
    EventsSubscriptionOrganizer::SubscribersCollection subscribersToAlert;
    bool isValidCollection = m_subscribersOrganizer->FetchRelevantSubscribers(a_event.Type(), a_event.Location(), subscribersToAlert);
    if(!isValidCollection)
    {
        throw std::runtime_error("An unknown internal error has occurred");
    }

    Alert(subscribersToAlert, a_event);
}


void EventsRouter::Alert(EventsSubscriptionOrganizer::SubscribersCollection& a_subscribersToAlert, const Event& a_event)
{
    m_eventsNotifier.Invoke(a_subscribersToAlert, a_event);
}

} // smartbuilding