#include "events_subscription_organizer.hpp"
#include <stdexcept> // std::runtime_error, std::invalid_argument
#include <list>
#include <unordered_map>
#include <deque>
#include <utility> // std::make_pair
#include <algorithm> // std::for_each, std::find
#include "isubscriber.hpp"
#include "isubscribable.hpp"
#include "event.hpp"
#include "subscription_location.hpp"


namespace smartbuilding
{

void EventsSubscriptionOrganizer::Subscribe(ISubscriber* a_toSubscribe, const Event::EventType& a_type, const SubscriptionLocation& a_intrestedLocation)
{
    if(!a_toSubscribe)
    {
        throw std::runtime_error("Null pointer error");
    }

    if(m_eventsSubscribersTable.find(a_type) == m_eventsSubscribersTable.end()) // Event not found
    {
        CreateEventEntry(a_type);
    }

    m_eventsSubscribersTable[a_type].push_back(std::make_pair(a_toSubscribe, a_intrestedLocation));
}


void EventsSubscriptionOrganizer::Unsubscribe(ISubscriber* a_toUnsubscribe, const Event::EventType& a_type)
{
    if(!a_toUnsubscribe)
    {
        throw std::runtime_error("Null pointer error");
    }

    if(m_eventsSubscribersTable.find(a_type) == m_eventsSubscribersTable.end()) // Event not found
    {
        throw std::invalid_argument("Event type not found error");
    }

    RemoveSubscriberFromEventList(m_eventsSubscribersTable[a_type], a_toUnsubscribe);
}


bool EventsSubscriptionOrganizer::FetchRelevantSubscribers(const Event::EventType& a_type, const Event::EventLocation& a_location, SubscribersContainer& a_relevantSubscribersContainer) noexcept
{
    if(m_eventsSubscribersTable.find(a_type) == m_eventsSubscribersTable.end()) // Event not found
    {
        return true;
    }

    try
    {
        std::list<SubscribersToIntrestedLocationsPair>& relatedSubscribersCollection = m_eventsSubscribersTable[a_type];
        std::for_each(relatedSubscribersCollection.begin(), relatedSubscribersCollection.end(), [&](const SubscribersToIntrestedLocationsPair& a_subscriberToIntrestedLocations)
        {
            if(IsIntrestedLocationBySubscriber(a_location, a_subscriberToIntrestedLocations.second))
            {
                a_relevantSubscribersContainer.push_back(a_subscriberToIntrestedLocations.first);
            }
        });
    }
    catch(const std::exception& ex)
    {
        // An error has occurred - cannot guarantee a valid container (could be an bad_alloc exception...)
        return false;
    }

    return true;
}


void EventsSubscriptionOrganizer::CreateEventEntry(const Event::EventType& a_type)
{
    m_eventsSubscribersTable[a_type] = std::list<SubscribersToIntrestedLocationsPair>();
}


void EventsSubscriptionOrganizer::RemoveSubscriberFromEventList(std::list<SubscribersToIntrestedLocationsPair>& a_list, ISubscriber* a_toRemove)
{
    auto itr = a_list.begin();
    auto endItr = a_list.end();

    for(; itr != endItr; ++itr)
    {
        SubscribersToIntrestedLocationsPair currentPair = *itr;
        if(currentPair.first == a_toRemove)
        {
            // Remove the SubscribersToLocationsPair from this event
            a_list.erase(itr);
            break;
        }
    }
}


bool EventsSubscriptionOrganizer::IsIntrestedLocationBySubscriber(const Event::EventLocation& a_eventLocation, const SubscriptionLocation& a_intrestedLocation) const noexcept
{
    if(a_intrestedLocation.IsAllFloors())
    {
        if(a_intrestedLocation.IsAllRooms())
        {
            return true;
        }
        else
        {
            return HasNumberSpecified(a_intrestedLocation.SpecifiedRooms(), a_eventLocation.Room());
        }
    }
    else
    {
        if(HasNumberSpecified(a_intrestedLocation.SpecifiedFloors(), a_eventLocation.Floor()))
        {
            if(a_intrestedLocation.IsAllRooms())
            {
                return true;
            }
            else
            {
                return HasNumberSpecified(a_intrestedLocation.SpecifiedRooms(), a_eventLocation.Room());
            }
        }
        else
        {
            return false;
        }
    }
}


bool EventsSubscriptionOrganizer::HasNumberSpecified(const std::vector<unsigned int>& a_allNumber, unsigned int a_number) const noexcept
{
    auto end = a_allNumber.end();
    if(std::find(a_allNumber.begin(), end, a_number) == end)
    {
        // The number has NOT specified by the subscriber
        return false;
    }

    return true;
}

} // smartbuilding
