#ifndef NM_EVENTS_ROUTER_HPP
#define NM_EVENTS_ROUTER_HPP


#include <unordered_map> // std::unordered_map
#include "isubscriber.hpp"
#include "events_dispatcher.hpp"
#include "event.hpp"


namespace smartbuilding
{
// Note: each pre-configured EventType of Event class would be added to the DB as a key, so the system be as extensible as possible
class EventsRouter
{
public:
    EventsRouter() = default;
    EventsRouter(const EventsRouter& a_other) = default;
    EventsRouter& operator=(const EventsRouter& a_other) = default;
    ~EventsRouter() = default;

    void AddEvent(Event::EventType);
    void RemoveEvent(Event::EventType);
    void SubscribeToEvent(ISubscriber* a_subscriber, Event::EventType a_type); //TODO: invoke the inner EventsDispatcher
    void UnsubscribeToEvent(ISubscriber* a_subscriber, Event::EventType a_type); //TODO: invoke the inner EventsDispatcher

    void RouteEvent(Event::EventType);

private:
    void Alert(Event::EventType); // TODO: used by the RouteEvent method, to call the inner EventDispatcher's invoke method

private:
    // TODO: find a way to aggregate another table in this tables (Events->floors&&rooms->Subscribers)
    // maybe: std::unordered_map<Event::EventType, std::unordered_map<floors||rooms,EventsDispatcher> ...
    std::unordered_map<Event::EventType,EventsDispatcher> m_EventsSubscribersTables;
};

} // smartbuilding


#endif // NM_EVENTS_ROUTER_HPP