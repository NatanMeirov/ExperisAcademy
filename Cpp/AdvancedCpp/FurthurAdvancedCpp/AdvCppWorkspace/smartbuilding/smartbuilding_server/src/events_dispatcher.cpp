#include "events_dispatcher.hpp"
#include <algorithm> // std::for_each
#include <type_traits> // std::is_same
#include <memory> // std::shared_ptr
#include "isubscriber.hpp"
#include "invoker_work.hpp"


namespace smartbuilding
{

EventsDispatcher::EventsDispatcher()
: m_invokers(advcpp::ShutdownPolicy<>(), WORKERS_QUEUE_SIZE)
{
}


EventsDispatcher::~EventsDispatcher()
{
    m_invokers.Shutdown();
}


template <typename C>
void EventsDispatcher::Invoke(C a_subscribersCollection, const Event& a_event) noexcept
{
    static_assert(std::is_same<typename C::value_type, ISubscriber*>::value, "C::value_type must be of type ISubscriber*");

    std::for_each(a_subscribersCollection.begin(), a_subscribersCollection.end(), [&](ISubscriber* a_subscriber)
    {
        try
        {
            std::shared_ptr<InvokerWork> invokeTask(new InvokerWork(a_subscriber, a_event));
            m_invokers.SubmitWork(invokeTask);
        }
        catch(...)
        {
            // For exception safety
        }
    });
}

} // smartbuilding