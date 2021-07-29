#ifndef NM_EVENTS_ROUTER_HPP
#define NM_EVENTS_ROUTER_HPP


#include <unordered_map> // std::unordered_map
#include <memory> // std::shared_ptr
#include "isubscriber.hpp"
#include "event.hpp"
#include "events_subscription_organizer.hpp"
#include "events_dispatcher.hpp"


namespace smartbuilding
{

class EventsRouter
{
public:
    EventsRouter(std::shared_ptr<EventsSubscriptionOrganizer> a_subscribersOrganizer);
    EventsRouter(const EventsRouter& a_other) = delete;
    EventsRouter& operator=(const EventsRouter& a_other) = delete;
    ~EventsRouter() = default;

    void RouteEvent(Event a_event); // Passes the event by copy (cannot ensure that the reference to the event is still valid)

private:
    void Alert(EventsSubscriptionOrganizer::SubscribersCollection& a_subscribersToAlert, const Event& a_event);

private:
    std::shared_ptr<EventsSubscriptionOrganizer> m_subscribersOrganizer; // The subscribers Database
    EventsDispatcher m_eventsNotifier;
};

} // smartbuilding


#endif // NM_EVENTS_ROUTER_HPP