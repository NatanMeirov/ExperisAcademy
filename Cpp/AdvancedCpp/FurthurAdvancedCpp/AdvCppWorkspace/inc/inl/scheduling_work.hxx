#ifndef NM_SCHEDULING_WORK_HXX
#define NM_SCHEDULING_WORK_HXX


#include <memory> // std::shared_ptr
#include <mutex> // std::mutex, std::lock_guard
#include "icallable.hpp"
#include "blocking_bounded_queue.hpp"
#include "blocking_bounded_queue_destruction_policies.hpp"
#include "two_way_multi_sync_handler.hpp"


namespace advcpp
{

template <typename QueueType>
SchedulingWork<QueueType>::SchedulingWork(std::shared_ptr<QueueType> a_worksQueue, std::shared_ptr<TwoWayMultiSyncHandler> a_twoWayMultiSyncHandler, std::shared_ptr<std::mutex> a_workersLock)
: m_worksQueue(a_worksQueue)
, m_twoWayMultiSyncHandler(a_twoWayMultiSyncHandler)
, m_workersLock(a_workersLock)
{
}


template <typename QueueType>
void SchedulingWork<QueueType>::operator()()
{
    while(true)
    {
        if(m_twoWayMultiSyncHandler->NotificationsCount() > 0) // The thread pool notified N task to stop their execution
        {
            // Double check lock
            std::lock_guard<std::mutex> guard(*m_workersLock);
            if(m_twoWayMultiSyncHandler->NotificationsCount() > 0)
            {
                m_twoWayMultiSyncHandler->AcceptNotification();
                break;
            }

            // Continue this iteration regularly
        }

        std::shared_ptr<ICallable> work;
        if(!m_worksQueue->Dequeue(work)) // Checking if the queue is not valid
        {
            break;
        }
        SafeExecute(work);
    }

    m_twoWayMultiSyncHandler->SignalBack(); // Signals that the worker has finished its work and can be removed successfully
}


template <typename QueueType>
void SchedulingWork<QueueType>::SafeExecute(std::shared_ptr<ICallable> a_work) const
{
    try
    {
        if(a_work)
        {
            (*a_work)();
        }
    }
    catch(...)
    {
        // For exception safety execution
    }
}

} // advcpp


#endif // NM_SCHEDULING_WORK_HXX
