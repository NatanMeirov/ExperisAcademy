#include "events_router.hpp"
#include "events_subscription_organizer.hpp"
#include "events_dispatcher.hpp"


namespace smartbuilding
{

EventsRouter::EventsRouter(EventsSubscriptionOrganizer& a_subscribersOrganizer)
: m_subscribersOrganizer(a_subscribersOrganizer)
, m_eventsNotifier()
{
}


void EventsRouter::RouteEvent(Event a_event)
{
    // TODO
}


void EventsRouter::Alert(const Event& a_event)
{
    // TODO
}

} // smartbuilding