#ifndef NM_ISUBSCRIBABLE_HPP
#define NM_ISUBSCRIBABLE_HPP


#include "isubscriber.hpp"
#include "event.hpp"
#include "subscription_location.hpp"


namespace smartbuilding
{

class ISubscribable
{
public:
    virtual ~ISubscribable() = default;
    virtual void Subscribe(ISubscriber* a_toSubscribe, const Event::EventType& a_type, const SubscriptionLocation& a_intrestedLocation) = 0;
    virtual void Unsubscribe(ISubscriber* a_toUnsubscribe, const Event::EventType& a_type) = 0;
};

} // smartbuilding


#endif // NM_ISUBSCRIBABLE_HPP