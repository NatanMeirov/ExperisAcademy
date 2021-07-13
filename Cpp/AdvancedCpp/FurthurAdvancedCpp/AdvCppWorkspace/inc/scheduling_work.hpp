#ifndef NM_SCHEDULING_WORK_HPP
#define NM_SCHEDULING_WORK_HPP


#include <memory> // std::shared_ptr
#include "icallable.hpp"
#include "blocking_bounded_queue.hpp"
#include "blocking_bounded_queue_destruction_policies.hpp"
#include "atomic_value.hpp"
#include "sync_handler.hpp"


namespace advcpp
{

class SchedulingWork : public ICallable
{
public:
    using WorksQueue = std::shared_ptr<BlockingBoundedQueue<std::shared_ptr<ICallable>, ClearPolicy<std::shared_ptr<ICallable>>>>;

    SchedulingWork(WorksQueue a_worksQueue, std::shared_ptr<SyncHandler> a_syncHandler);
    SchedulingWork(const SchedulingWork& a_other) = delete;
    SchedulingWork& operator=(const SchedulingWork& a_other) = delete;
    ~SchedulingWork() = default;

    virtual void operator()() override;

    bool HasFinished() { return m_hasFinished.Check(); }

private:
    WorksQueue m_worksQueue;
    std::shared_ptr<SyncHandler> m_syncHandler;
    AtomicFlag m_hasFinished;
};

} // advcpp


#endif // NM_SCHEDULING_WORK_HPP