#ifndef NM_EVENTS_DISPATCHER_HPP
#define NM_EVENTS_DISPATCHER_HPP


#include <list> // std::list
#include "isubscriber.hpp"
#include "event.hpp"
#include "thread_pool.hpp"
#include "thread_pool_destruction_policies.hpp"


namespace smartbuilding
{

class EventsDispatcher
{
public:
    EventsDispatcher();
    EventsDispatcher(const EventsDispatcher& a_other) = delete;
    EventsDispatcher& operator=(const EventsDispatcher& a_other) = delete;
    ~EventsDispatcher();

    // Concept of C: C must be an iterable container (implement begin() and end()), must have value_type info (typedef), and C::value_type must be ISubscriber*
    template <typename C>
    void Invoke(C a_subscribersCollection, const Event& a_event) noexcept;

private:
    static const unsigned int WORKERS_QUEUE_SIZE = 100; // TODO: in version 2, read this constant from a configuration file

private:
    advcpp::ThreadPool<advcpp::ShutdownPolicy<>> m_invokers;
};

} // smartbuilding


#endif // NM_EVENTS_DISPATCHER_HPP