#include "events_dispatcher.hpp"
#include <list> // std::list
#include <stdexcept> // std::runtime_error
#include <algorithm> // std::for_each
#include "isubscriber.hpp"
#include "isubscribable.hpp"

namespace smartbuilding
{

EventsDispatcher::EventsDispatcher()
: m_subscribers()
{
}


void EventsDispatcher::Subscribe(ISubscriber* a_toSubscribe)
{
    if(!a_toSubscribe)
    {
        throw std::runtime_error("Null pointer error");
    }

    m_subscribers.push_back(a_toSubscribe);
}


void EventsDispatcher::Unsubscribe(ISubscriber* a_toUnsubscribe)
{
    if(!a_toUnsubscribe)
    {
        throw std::runtime_error("Null pointer error");
    }

    m_subscribers.remove(a_toUnsubscribe);
}


void EventsDispatcher::Invoke()
{
    std::for_each(m_subscribers.begin(), m_subscribers.end(), [](ISubscriber* a_toInvoke)
    {
        a_toInvoke->Notify();
    });
}

} // smartbuilding