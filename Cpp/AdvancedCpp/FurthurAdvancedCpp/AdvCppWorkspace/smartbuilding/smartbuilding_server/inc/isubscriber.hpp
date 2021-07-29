#ifndef NM_ISUBSCRIBER_HPP
#define NM_ISUBSCRIBER_HPP


#include "event.hpp"


namespace smartbuilding
{

class ISubscriber
{
public:
    virtual ~ISubscriber() = default;
    virtual void Notify(Event a_event) = 0; // Passes the event by copy (cannot ensure that the reference to the event is still valid)
};

} // smartbuilding


#endif // NM_ISUBSCRIBER_HPP