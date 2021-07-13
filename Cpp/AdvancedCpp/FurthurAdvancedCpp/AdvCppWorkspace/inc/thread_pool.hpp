#ifndef NM_THREAD_POOL_HPP
#define NM_THREAD_POOL_HPP


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


namespace advcpp
{

class ThreadPool
{
public:
    using Work = std::shared_ptr<ICallable>;

    explicit ThreadPool(size_t a_workersNumber);
    ThreadPool(const ThreadPool& a_other) = delete;
    ThreadPool& operator=(const ThreadPool& a_other) = delete;
    ~ThreadPool();

    void AddWorkers(size_t a_workers);
    void RemoveWorkers(size_t a_workers);

    void SubmitWork(Work a_work);

    void Shutdown(); // Executes all pending works, but user cannot add new works
    void ShutdownImmediate(); // Does not accept new works, does not execute any pending work, but complete works that were already started

private:
    static constexpr size_t WORKS_QUEUE_SIZE = 39;

private: // Order is important!
    std::shared_ptr<BlockingBoundedQueue<Work, ClearPolicy<Work>>> m_worksQueue;
    std::shared_ptr<SyncHandler> m_syncHandler;
    Work m_mainSchedulingWork;
    ThreadGroup<DetachPolicy> m_workers;
    AtomicFlag m_isStopRequired;
};

} // advcpp


#endif // NM_THREAD_POOL_HPP