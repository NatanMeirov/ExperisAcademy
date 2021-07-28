#ifndef NM_EVENTS_ROUTER_HPP
#define NM_EVENTS_ROUTER_HPP


#include <unordered_map> // std::unordered_map
#include "isubscriber.hpp"
#include "event.hpp"
#include "events_subscription_organizer.hpp"
#include "events_dispatcher.hpp"


namespace smartbuilding
{

class EventsRouter
{
public:
    EventsRouter(EventsSubscriptionOrganizer& a_subscribersOrganizer);
    EventsRouter(const EventsRouter& a_other) = delete;
    EventsRouter& operator=(const EventsRouter& a_other) = delete;
    ~EventsRouter() = default;

    void RouteEvent(Event a_event); // Pass the event by copy (cannot ensure that the reference to the event is still valid)

private:
    void Alert(const Event& a_event); // Use this in the RouteEvent method, to call the inner EventDispatcher's invoke method

private:
    EventsSubscriptionOrganizer& m_subscribersOrganizer;
    EventsDispatcher m_eventsNotifier;
};

} // smartbuilding


#endif // NM_EVENTS_ROUTER_HPP