#ifndef NMM_THREAD_POOL_HXX
#define NMM_THREAD_POOL_HXX


#include <cstddef> // size_t
#include <memory> // std::shared_ptr, std::make_shared
#include <stdexcept> // std::runtime_error
#include <mutex> // std::mutex, std::lock_guard
#include "blocking_bounded_queue.hpp"
#include "thread.hpp"
#include "thread_group.hpp"
#include "thread_destruction_policies.hpp"
#include "icallable.hpp"
#include "blocking_bounded_queue.hpp"
#include "blocking_bounded_queue_destruction_policies.hpp"
#include "atomic_value.hpp"
#include "two_way_multi_sync_handler.hpp"
#include "works_scheduler.hpp"
#include "suicide_mission.hpp"
#include "works_enqueuer.hpp"


namespace advcpp
{

template <typename DestructionPolicy, typename QueueTypeDestructionPolicy, typename QueueType>
ThreadPool<DestructionPolicy,QueueTypeDestructionPolicy,QueueType>::ThreadPool(DestructionPolicy a_destructionPolicy, size_t a_worksQueueSize, size_t a_workersNumber)
: m_worksQueue(new QueueType(a_worksQueueSize, QueueTypeDestructionPolicy()))
, m_twoWayMultiSyncHandler(new TwoWayMultiSyncHandler())
, m_workersLock(new std::mutex())
, m_mainWorksScheduler(new WorksScheduler<QueueTypeDestructionPolicy,QueueType>(m_worksQueue, m_twoWayMultiSyncHandler, m_workersLock))
, m_workers(m_mainWorksScheduler, a_workersNumber, CancelPolicy())
, m_operationsLock()
, m_isStopRequired(false)
, m_destructionPolicy(a_destructionPolicy)
{
}


template <typename DestructionPolicy, typename QueueTypeDestructionPolicy, typename QueueType>
ThreadPool<DestructionPolicy,QueueTypeDestructionPolicy,QueueType>::~ThreadPool()
{
    m_destructionPolicy(*this);
}


template <typename DestructionPolicy, typename QueueTypeDestructionPolicy, typename QueueType>
void ThreadPool<DestructionPolicy,QueueTypeDestructionPolicy,QueueType>::SubmitWork(Work a_work)
{
    if(HasStopped())
    {
        throw std::runtime_error("Failed while tried to submit new work (because of previous Shutdown call)");
    }

    InsertWorkToQueue(a_work);
}


template <typename DestructionPolicy, typename QueueTypeDestructionPolicy, typename QueueType>
void ThreadPool<DestructionPolicy,QueueTypeDestructionPolicy,QueueType>::AddWorkers(size_t a_workers)
{
    if(HasStopped())
    {
        throw std::runtime_error("Failed while tried to add new workers (because of previous Shutdown call)");
    }

    // Lock the other pool's operations
    std::lock_guard<std::mutex> guard(m_operationsLock);

    m_workers.Add(a_workers);
}


template <typename DestructionPolicy, typename QueueTypeDestructionPolicy, typename QueueType>
void ThreadPool<DestructionPolicy,QueueTypeDestructionPolicy,QueueType>::RemoveWorkers(size_t a_workers)
{
    if(HasStopped())
    {
        throw std::runtime_error("Failed while tried to remove existing workers (because of previous Shutdown call)");
    }

    // Lock the other pool's operations
    std::lock_guard<std::mutex> guard(m_operationsLock);

    size_t workersToRemove = std::min(a_workers, m_workers.Size());
    StopWorkers(workersToRemove); // Stops all workers (m_workers.Size()) if a_workers > m_workers.Size()
    m_workers.Remove(workersToRemove);
}


template <typename DestructionPolicy, typename QueueTypeDestructionPolicy, typename QueueType>
void ThreadPool<DestructionPolicy,QueueTypeDestructionPolicy,QueueType>::Shutdown()
{
    Stop();
    SoftShutdown();
    m_workers.Remove(m_workers.Size());
}


template <typename DestructionPolicy, typename QueueTypeDestructionPolicy, typename QueueType>
void ThreadPool<DestructionPolicy,QueueTypeDestructionPolicy,QueueType>::ShutdownImmediate()
{
    Stop();
    ForceShutdown();
    m_workers.Remove(m_workers.Size());
}


template <typename DestructionPolicy, typename QueueTypeDestructionPolicy, typename QueueType>
size_t ThreadPool<DestructionPolicy,QueueTypeDestructionPolicy,QueueType>::WorkersCount()
{
    return m_workers.Size();
}


template <typename DestructionPolicy, typename QueueTypeDestructionPolicy, typename QueueType>
size_t ThreadPool<DestructionPolicy,QueueTypeDestructionPolicy,QueueType>::PendingWorksCount() const
{
    return m_worksQueue->Size();
}


template <typename DestructionPolicy, typename QueueTypeDestructionPolicy, typename QueueType>
void ThreadPool<DestructionPolicy,QueueTypeDestructionPolicy,QueueType>::InsertWorkToQueue(Work a_workToInsert)
{
    Work worksEnqueuer(new WorksEnqueuer<QueueTypeDestructionPolicy,QueueType>(m_worksQueue, a_workToInsert));
    Thread<DetachPolicy> workEnqueueTask(worksEnqueuer, DetachPolicy());
    workEnqueueTask.Detach();
}


template <typename DestructionPolicy, typename QueueTypeDestructionPolicy, typename QueueType>
void ThreadPool<DestructionPolicy,QueueTypeDestructionPolicy,QueueType>::Stop()
{
    m_isStopRequired.True();
}


template <typename DestructionPolicy, typename QueueTypeDestructionPolicy, typename QueueType>
bool ThreadPool<DestructionPolicy,QueueTypeDestructionPolicy,QueueType>::HasStopped() const
{
    return m_isStopRequired.Check();
}


template <typename DestructionPolicy, typename QueueTypeDestructionPolicy, typename QueueType>
void ThreadPool<DestructionPolicy,QueueTypeDestructionPolicy,QueueType>::SoftShutdown()
{
    while(!m_worksQueue->IsEmpty() && m_workers.Size() > 0); // Pooling - to make sure that the workers have consumed all the works, and there are ready to be stopped (and make sure that there are workers to consume the works... must make sure to not wait for nothing)
    StopWorkers(m_workers.Size()); // Stop all workers
}


template <typename DestructionPolicy, typename QueueTypeDestructionPolicy, typename QueueType>
void ThreadPool<DestructionPolicy,QueueTypeDestructionPolicy,QueueType>::ForceShutdown()
{
    StopWorkers(m_workers.Size()); // Stop all workers
}


template <typename DestructionPolicy, typename QueueTypeDestructionPolicy, typename QueueType>
void ThreadPool<DestructionPolicy,QueueTypeDestructionPolicy,QueueType>::StopWorkers(size_t a_workersToStop)
{
    m_twoWayMultiSyncHandler->SetWantedSignalsBack(a_workersToStop);
    m_twoWayMultiSyncHandler->Notify(a_workersToStop); // Notify N workers

    Work suicideMission(new SuicideMission()); // Using the suicide mission (that throws) to make sure that N workers are working on something, and NOT waiting on the Dequeue, so they are cancelable
    for(size_t i = 0; i < a_workersToStop; ++i)
    {
        InsertWorkToQueue(suicideMission);
    }

    m_twoWayMultiSyncHandler->WaitForAllSignalsBack(); // A blocking wait (no polling is required)
    m_twoWayMultiSyncHandler->ResetNotifications();
}


template <typename DestructionPolicy, typename QueueTypeDestructionPolicy, typename QueueType>
void ThreadPool<DestructionPolicy,QueueTypeDestructionPolicy,QueueType>::ConditionalShutdown() noexcept
{
    if(!HasStopped())
    {
        Shutdown();
    }
}


template <typename DestructionPolicy, typename QueueTypeDestructionPolicy, typename QueueType>
void ThreadPool<DestructionPolicy,QueueTypeDestructionPolicy,QueueType>::ConditionalShutdownImmidiate() noexcept
{
    if(!HasStopped())
    {
        ShutdownImmediate();
    }
}

} // advcpp


#endif // NMM_THREAD_POOL_HXX