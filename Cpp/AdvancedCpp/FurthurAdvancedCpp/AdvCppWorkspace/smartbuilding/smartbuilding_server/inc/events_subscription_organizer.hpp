#ifndef NM_EVENTS_SUBSCRIPTION_ORGANIZER_HPP
#define NM_EVENTS_SUBSCRIPTION_ORGANIZER_HPP


#include <cstddef> // size_t
#include <list> // std::list
#include <unordered_map> // std::unordered_map
#include <deque> // std::deque

#include "isubscriber.hpp"
#include "isubscribable.hpp"
#include "event.hpp"
#include "event_location.hpp"
#include "subscription_location.hpp"


namespace smartbuilding
{

// An extensible lazy initialization events subscription organizer
// Note: each pre-configured EventType (through the config file) - would be added to the DB as a key, so the system should be as extensible as possible
class EventsSubscriptionOrganizer : public ISubscribable
{
public:
    using SubscribersCollection = std::deque<ISubscriber*>;
    using SubscribersToIntrestedLocationsPair = std::pair<ISubscriber*, SubscriptionLocation>;

    EventsSubscriptionOrganizer() = default;
    EventsSubscriptionOrganizer(const EventsSubscriptionOrganizer& a_other) = delete;
    EventsSubscriptionOrganizer& operator=(const EventsSubscriptionOrganizer& a_other) = delete;
    ~EventsSubscriptionOrganizer() = default;

    virtual void Subscribe(ISubscriber* a_toSubscribe, const Event::EventType& a_type, const SubscriptionLocation& a_intrestedLocation) override;
    virtual void Unsubscribe(ISubscriber* a_toUnsubscribe, const Event::EventType& a_type) override;

    // An important data fetching on run-time method - exception safety guaranteed (no throws even if the event is not found -> in this case: true would be returned, and the container would be empty [container is in valid state])
    // [returns true if the container is fully valid, else returns false]
    bool FetchRelevantSubscribers(const Event::EventType& a_type, const EventLocation& a_location, SubscribersCollection& a_relevantSubscribersContainer) noexcept;

private:
    void CreateEventEntry(const Event::EventType& a_type);
    void RemoveSubscriberFromEventList(std::list<SubscribersToIntrestedLocationsPair>& a_list, ISubscriber* a_toRemove);
    bool IsIntrestedLocationBySubscriber(const EventLocation& a_eventLocation, const SubscriptionLocation& a_intrestedLocation) const noexcept;
    bool HasNumberSpecified(const std::vector<unsigned int>& a_allNumber, unsigned int a_number) const noexcept;

private:
    std::unordered_map<Event::EventType, std::list<SubscribersToIntrestedLocationsPair>> m_eventsSubscribersTable;
};

} // smartbuilding


#endif // NM_EVENTS_SUBSCRIPTION_ORGANIZER_HPP