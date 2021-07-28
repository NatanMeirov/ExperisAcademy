#ifndef NM_EVENTS_SUBSCRIPTION_ORGANIZER_HPP
#define NM_EVENTS_SUBSCRIPTION_ORGANIZER_HPP


#include <list> // std::list
#include <unordered_map> // std::unordered_map
#include <deque> // std::deque
#include "isubscriber.hpp"
#include "isubscribable.hpp"
#include "event.hpp"
#include "event_location.hpp"


namespace smartbuilding
{

// An extensible lazy initialization events subscription organizer
// Note: each pre-configured EventType (through the config file) - would be added to the DB as a key, so the system should be as extensible as possible
class EventsSubscriptionOrganizer : public ISubscribable
{
public:
    using SubscribersCollection = std::deque<ISubscriber*>;

    EventsSubscriptionOrganizer() = default;
    EventsSubscriptionOrganizer(const EventsSubscriptionOrganizer& a_other) = delete;
    EventsSubscriptionOrganizer& operator=(const EventsSubscriptionOrganizer& a_other) = delete;
    ~EventsSubscriptionOrganizer() = default;

    virtual void Subscribe(ISubscriber* a_toSubscribe, const Event::EventType& a_type /*FIXME: ADD SUBSCRIBING LOCATION!*/) override;
    virtual void Unsubscribe(ISubscriber* a_toUnsubscribe, const Event::EventType& a_type /*FIXME: ADD SUBSCRIBING LOCATION!*/) override;

    SubscribersCollection FetchRelevantSubscribers(const Event::EventType& a_type, const EventLocation& a_location);

private:
    void CreateEventEntry(const Event::EventType& a_type);

private:
    std::unordered_map<Event::EventType, std::list<ISubscriber*>> m_EventsSubscribersTable;
    // TODO: find a way to aggregate another table in this tables (Events->floors&&rooms->Subscribers)
};

} // smartbuilding


#endif // NM_EVENTS_SUBSCRIPTION_ORGANIZER_HPP