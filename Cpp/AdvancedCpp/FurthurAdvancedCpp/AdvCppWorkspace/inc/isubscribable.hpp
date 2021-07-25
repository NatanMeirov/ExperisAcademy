#ifndef NM_ISUBSCRIBABLE_HPP
#define NM_ISUBSCRIBABLE_HPP


#include <memory> // std::shared_ptr
#include "isubscriber.hpp"


namespace smartbuilding
{

class ISubscribable
{
public:
    virtual ~ISubscribable() = default;
    virtual void Subscribe(std::shared_ptr<ISubscriber> a_toSubscribe) = 0;
    virtual void Unsubscribe(std::shared_ptr<ISubscriber> a_toUnsubscribe) = 0;
};

} // smartbuilding


#endif // NM_ISUBSCRIBABLE_HPP