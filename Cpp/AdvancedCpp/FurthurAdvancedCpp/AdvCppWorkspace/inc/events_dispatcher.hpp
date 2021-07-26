#ifndef NM_EVENTS_DISPATCHER_HPP
#define NM_EVENTS_DISPATCHER_HPP


#include <list> // std::list
#include "isubscriber.hpp"
#include "isubscribable.hpp"


namespace smartbuilding
{

class EventsDispatcher : public ISubscribable
{
public:
    EventsDispatcher();
    EventsDispatcher(const EventsDispatcher& a_other) = delete;
    EventsDispatcher& operator=(const EventsDispatcher& a_other) = delete;
    ~EventsDispatcher() = default;

    virtual void Subscribe(ISubscriber* a_toSubscribe) override;
    virtual void Unsubscribe(ISubscriber* a_toUnsubscribe) override;
    void Invoke();

private:
    std::list<ISubscriber*> m_subscribers;
};

} // smartbuilding


#endif // NM_EVENTS_DISPATCHER_HPP