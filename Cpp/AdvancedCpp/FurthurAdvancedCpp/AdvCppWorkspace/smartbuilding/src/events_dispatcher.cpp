#include "events_dispatcher.hpp"
#include <algorithm> // std::for_each
#include <type_traits> // std::is_same
#include "isubscriber.hpp"

namespace smartbuilding
{

template <typename C>
void EventsDispatcher::Invoke(C a_subscribersCollection, Event::DataPayload a_data)
{
    static_assert(std::is_same<typename C::value_type, ISubscriber*>::value, "C::value_type must be of type ISubscriber*");

    std::for_each(a_subscribersCollection.begin(), a_subscribersCollection.end(), [&a_data](ISubscriber* a_toInvoke)
    {
        a_toInvoke->Notify(a_data);
    });
}

} // smartbuilding