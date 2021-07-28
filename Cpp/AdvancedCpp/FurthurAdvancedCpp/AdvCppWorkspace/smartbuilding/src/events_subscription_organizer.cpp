#include "events_subscription_organizer.hpp"
#include <stdexcept> // std::runtime_error, std::invalid_argument
#include <list>
#include <unordered_map>
#include <deque>
#include "isubscriber.hpp"
#include "isubscribable.hpp"


namespace smartbuilding
{

void EventsSubscriptionOrganizer::Subscribe(ISubscriber* a_toSubscribe, const Event::EventType& a_type)
{
    if(!a_toSubscribe)
    {
        throw std::runtime_error("Null pointer error");
    }

    if(m_EventsSubscribersTable.find(a_type) == m_EventsSubscribersTable.end()) // Event not found
    {
        CreateEventEntry(a_type);
    }

    m_EventsSubscribersTable[a_type].push_back(a_toSubscribe);
}


void EventsSubscriptionOrganizer::Unsubscribe(ISubscriber* a_toUnsubscribe, const Event::EventType& a_type)
{
    if(!a_toUnsubscribe)
    {
        throw std::runtime_error("Null pointer error");
    }

    if(m_EventsSubscribersTable.find(a_type) == m_EventsSubscribersTable.end()) // Event not found
    {
        throw std::invalid_argument("Event type not found error");
    }

    m_EventsSubscribersTable[a_type].remove(a_toUnsubscribe);
}


EventsSubscriptionOrganizer::SubscribersCollection EventsSubscriptionOrganizer::FetchRelevantSubscribers(const Event::EventType& a_type, const EventLocation& a_location)
{
    //TODO!
}


void EventsSubscriptionOrganizer::CreateEventEntry(const Event::EventType& a_type)
{
    m_EventsSubscribersTable[a_type] = std::list<ISubscriber*>();
}

} // smartbuilding
