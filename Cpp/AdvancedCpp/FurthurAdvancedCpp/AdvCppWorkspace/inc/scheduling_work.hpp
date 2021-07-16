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

// Concept of QueueType: QueueType must implement Enqueue, Dequeue, IsEmpty and Size methods (Suggestion: these methods should be multithreaded-safe!),
// and its T MUST be std::shared_ptr of type ICallable (QueueType< T = std::shared_ptr<ICallable> >),
template <typename QueueType = BlockingBoundedQueue<std::shared_ptr<ICallable>, ClearPolicy<std::shared_ptr<ICallable>>>>
class SchedulingWork : public ICallable
{
public:
    SchedulingWork(std::shared_ptr<QueueType> a_worksQueue, std::shared_ptr<TwoWayMultiSyncHandler> a_twoWayMultiSyncHandler, std::shared_ptr<std::mutex> a_workersLock);
    SchedulingWork(const SchedulingWork& a_other) = delete;
    SchedulingWork& operator=(const SchedulingWork& a_other) = delete;
    ~SchedulingWork() = default;

    virtual void operator()() override;

private:
    void SafeExecute(std::shared_ptr<ICallable> a_work) const;

private:
    std::shared_ptr<QueueType> m_worksQueue;
    std::shared_ptr<TwoWayMultiSyncHandler> m_twoWayMultiSyncHandler;
    std::shared_ptr<std::mutex> m_workersLock;
};

} // advcpp


#include "inl/scheduling_work.hxx"


#endif // NM_SCHEDULING_WORK_HPP