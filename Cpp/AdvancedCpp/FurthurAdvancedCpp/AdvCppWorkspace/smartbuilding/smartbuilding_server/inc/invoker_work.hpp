#ifndef NM_INVOKER_WORK_HPP
#define NM_INVOKER_WORK_HPP


#include "icallable.hpp"
#include "isubscriber.hpp"
#include "event.hpp"


namespace smartbuilding
{

class InvokerWork : public advcpp::ICallable
{
public:
    InvokerWork(ISubscriber* a_toInvoke, const Event& a_event) : m_subscriber(a_toInvoke), m_event(a_event) {}

    virtual void operator()() override
    {
        m_subscriber->Notify(m_event);
    }

private:
    ISubscriber* m_subscriber;
    Event m_event;
};

} // smartbuilding


#endif // NM_INVOKER_WORK_HPP