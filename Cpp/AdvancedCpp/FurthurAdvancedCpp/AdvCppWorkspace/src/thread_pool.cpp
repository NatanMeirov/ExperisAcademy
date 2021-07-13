#include "thread_pool.hpp"
#include "blocking_bounded_queue.hpp"
#include "thread_group.hpp"
#include <cstddef> // size_t
#include <memory> // std::shared_ptr
#include "thread_group.hpp"
#include "thread_destruction_policies.hpp"
#include "icallable.hpp"
#include "blocking_bounded_queue.hpp"
#include "blocking_bounded_queue_destruction_policies.hpp"
#include "atomic_value.hpp"
#include "sync_handler.hpp"
#include "scheduling_work.hpp"


advcpp::ThreadPool::ThreadPool(size_t a_workersNumber)
: m_worksQueue(new BlockingBoundedQueue<Work, ClearPolicy<Work>>(WORKS_QUEUE_SIZE, ClearPolicy<Work>()))
, m_syncHandler(new SyncHandler())
, m_mainSchedulingWork(new SchedulingWork(m_worksQueue, m_syncHandler))
, m_workers(m_mainSchedulingWork, a_workersNumber, DetachPolicy())
, m_isStopRequired(false)
{
}


void advcpp::ThreadPool::SubmitWork(Work a_work)
{
    if(!m_isStopRequired.Check())
    {
        m_worksQueue->Enqueue(a_work);
    }
}


void advcpp::ThreadPool::Shutdown()
{
    m_isStopRequired.True();
}


void advcpp::ThreadPool::AddWorkers(size_t a_workers)
{
    m_workers.Add(a_workers);
}


void advcpp::ThreadPool::RemoveWorkers(size_t a_workers)
{
    m_workers.Remove(a_workers);
}


void advcpp::ThreadPool::ShutdownImmediate()
{
    m_syncHandler->Signal();
    m_isStopRequired.True();
}
