#ifndef NM_THREAD_POOL_HPP
#define NM_THREAD_POOL_HPP


#include <cstddef> // size_t
#include <memory> // std::shared_ptr
#include <thread> // std::thread::hardware_concurrency()
#include <mutex> // std::mutex
#include <vector> // std::vector
#include "thread.hpp"
#include "thread_destruction_policies.hpp"
#include "thread_group.hpp"
#include "icallable.hpp"
#include "blocking_bounded_queue.hpp"
#include "blocking_bounded_queue_destruction_policies.hpp"
#include "atomic_value.hpp"
#include "works_scheduler.hpp"
#include "two_way_multi_sync_handler.hpp"


namespace advcpp
{

// Concept of DestructionPolicy: policy must be copy-constructable
// The destruction policy is a FUNCTOR (implements operator() and get 1 param: Thread& obj), to be used as an instructions to know which action the Thread
// object should call on itself when it is in a destruction stage
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Concept of QueueTypeDestructionPolicy: must be a destruction policy of the given Queue type, and must be a destruction policy of type T = std::shared_ptr<ICallable>
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Concept of QueueType: QueueType must implement Enqueue, Dequeue, IsEmpty and Size methods (Suggestion: these methods should be multithreaded-safe!),
// and its T MUST be std::shared_ptr of type ICallable (QueueType< T = std::shared_ptr<ICallable> >),
// and it must implement a C'tor of: {size_t, QueueTypeDestructionPolicy<std::shared_ptr<ICallable>>}
template <typename DestructionPolicy, typename QueueTypeDestructionPolicy = ClearPolicy<std::shared_ptr<ICallable>>, typename QueueType = BlockingBoundedQueue<std::shared_ptr<ICallable>, QueueTypeDestructionPolicy>>
class ThreadPool
{
    friend DestructionPolicy;
public:
    using Work = std::shared_ptr<ICallable>;

    ThreadPool(DestructionPolicy a_destructionPolicy, size_t a_worksQueueSize, size_t a_workersNumber = std::thread::hardware_concurrency());
    ThreadPool(const ThreadPool& a_other) = delete;
    ThreadPool& operator=(const ThreadPool& a_other) = delete;
    ~ThreadPool();

    void AddWorkers(size_t a_workers);
    void RemoveWorkers(size_t a_workers);

    void SubmitWork(Work a_work);

    void Shutdown(); // Executes all pending works, but user cannot add new works
    void ShutdownImmediate(); // Does not accept new works, does not execute any pending work, but complete works that were already started

    size_t WorkersCount();
    size_t PendingWorksCount() const;

private:
    void InsertWorkToQueueAsync(Work a_workToInsert);
    bool HasDoneEnqueueAllWorksAsync() const;
    void Stop();
    bool HasStopped() const;
    void SoftShutdown();
    void ForceShutdown();
    void StopWorkers(size_t a_workersToStop);

    // For policy uses:
    void ConditionalShutdown() noexcept;
    void ConditionalShutdownImmidiate() noexcept;

private: // Order is important!
    std::shared_ptr<QueueType> m_worksQueue;
    std::shared_ptr<TwoWayMultiSyncHandler> m_twoWayMultiSyncHandler;
    std::shared_ptr<std::mutex> m_workersLock;
    Work m_mainWorksScheduler;
    ThreadGroup<CancelPolicy> m_workers;
    std::vector<std::shared_ptr<Thread<DetachPolicy>>> m_enqueueWorkThreads;
    std::mutex m_operationsLock;
    AtomicFlag m_isStopRequired;
    DestructionPolicy m_destructionPolicy;
};

} // advcpp


#include "inl/thread_pool.hxx"


#endif // NM_THREAD_POOL_HPP