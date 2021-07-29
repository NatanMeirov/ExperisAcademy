#ifndef NM_EVENTS_DISPATCHER_HPP
#define NM_EVENTS_DISPATCHER_HPP


#include <list> // std::list
#include "isubscriber.hpp"
#include "event.hpp"


namespace smartbuilding
{

class EventsDispatcher
{
public:
    EventsDispatcher() = default;
    EventsDispatcher(const EventsDispatcher& a_other) = delete;
    EventsDispatcher& operator=(const EventsDispatcher& a_other) = delete;
    ~EventsDispatcher() = default;

    // Concept of C: C must be an iterable container (implement begin() and end()), must have value_type info (typedef), and C::value_type must be ISubscriber*
    template <typename C>
    void Invoke(C a_subscribersCollection, const Event& a_event);
};

} // smartbuilding


#endif // NM_EVENTS_DISPATCHER_HPP