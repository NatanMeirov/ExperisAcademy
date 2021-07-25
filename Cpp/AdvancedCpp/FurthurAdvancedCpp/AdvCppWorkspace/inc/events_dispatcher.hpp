#ifndef NM_EVENTS_DISPATCHER_HPP
#define NM_EVENTS_DISPATCHER_HPP


#include <list> // std::list
#include <memory> // std::shared_ptr
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

    virtual void Subscribe(std::shared_ptr<ISubscriber> a_toSubscribe) override;
    virtual void Unsubscribe(std::shared_ptr<ISubscriber> a_toUnsubscribe) override;
    void Invoke();

private:
    std::list<std::shared_ptr<ISubscriber>> m_subscribers;
};

} // smartbuilding


#endif // NM_EVENTS_DISPATCHER_HPP