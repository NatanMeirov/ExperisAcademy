#ifndef NM_ISUBSCRIBABLE_HPP
#define NM_ISUBSCRIBABLE_HPP


#include "isubscriber.hpp"


namespace smartbuilding
{

class ISubscribable
{
public:
    virtual ~ISubscribable() = default;
    virtual void Subscribe(ISubscriber* a_toSubscribe) = 0;
    virtual void Unsubscribe(ISubscriber* a_toUnsubscribe) = 0;
};

} // smartbuilding


#endif // NM_ISUBSCRIBABLE_HPP