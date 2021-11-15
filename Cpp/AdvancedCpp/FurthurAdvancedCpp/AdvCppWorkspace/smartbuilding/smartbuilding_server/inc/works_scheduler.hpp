#ifndef NM_SCHEDULING_WORK_HPP
#define NM_SCHEDULING_WORK_HPP


#include <memory> // std::shared_ptr
#include <mutex> // std::mutex
#include "icallable.hpp"
#include "blocking_bounded_queue.hpp"
#include "blocking_bounded_queue_destruction_policies.hpp"
#include "two_way_multi_sync_handler.hpp"


namespace advcpp
{

// Concept of QueueTypeDestructionPolicy: must be a destruction policy of the given Queue type, and must be a destruction policy of type T = std::shared_ptr<ICallable>
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Concept of QueueType: QueueType must implement Enqueue, Dequeue, IsEmpty and Size methods (Suggestion: these methods should be multithreaded-safe!),
// and its T MUST be std::shared_ptr of type ICallable (QueueType< T = std::shared_ptr<ICallable> >),
// and it must implement a C'tor of: {size_t, QueueTypeDestructionPolicy<std::shared_ptr<ICallable>>}
template <typename QueueTypeDestructionPolicy = ClearPolicy<std::shared_ptr<ICallable>>, typename QueueType = BlockingBoundedQueue<std::shared_ptr<ICallable>, QueueTypeDestructionPolicy>>
class WorksScheduler : public ICallable
{
public:
    WorksScheduler(std::shared_ptr<QueueType> a_worksQueue, std::shared_ptr<TwoWayMultiSyncHandler> a_twoWayMultiSyncHandler, std::shared_ptr<std::mutex> a_workersLock);
    WorksScheduler(const WorksScheduler& a_other) = delete;
    WorksScheduler& operator=(const WorksScheduler& a_other) = delete;
    ~WorksScheduler() = default;

    virtual void operator()() override;

private:
    void SafeExecute(std::shared_ptr<ICallable> a_work) const;

private:
    std::shared_ptr<QueueType> m_worksQueue;
    std::shared_ptr<TwoWayMultiSyncHandler> m_twoWayMultiSyncHandler;
    std::shared_ptr<std::mutex> m_workersLock;
};

} // advcpp


#include "inl/works_scheduler.hxx"


#endif // NM_SCHEDULING_WORK_HPP
